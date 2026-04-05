# Engine

`Engine` is the central orchestrator of `Falcon.lib`. It owns every subsystem pointer, controls the three per-frame update paths, and manages the complete lifecycle from first allocation to final shutdown.

---

## Table of Contents

- [Class Interface](#class-interface)
- [Subsystem Members](#subsystem-members)
- [State Enum](#state-enum)
- [Public API](#public-api)
- [Update Paths](#update-paths)
- [Timer](#timer)
- [WindowClass](#windowclass)
- [InputReceiver](#inputreceiver)
- [ThreadPool](#threadpool)
- [Log](#log)

---

## Class Interface

```cpp
// Falcon/System/Engine.h  —  namespace gameLoop
class Engine {
    ThreadPool*          m_threads;
    Timer*               m_timer;
    Renderer*            m_renderer;
    WindowClass*         m_window;
    InputReceiver*       m_input;
    Scene::SceneGraph*   m_scene;
    Rendering::Octree*   m_octree;
    ParticleSystem*      m_particleSystem;
    STATE                m_currentState;

public:
    bool           m_gameCrashed, m_windowClosed;
    CAudioEngine   m_audio;           // value member — not a pointer

    bool   Initialize();
    void   MenuUpdate();
    void   PauseUpdate();
    void   IngameUpdate();
    void   ProcessInputs(float dt);   // stub — reserved for future use

    UI::UI_Manager* GetUI();
    WindowClass*    GetWindow();
    InputReceiver*  GetInput();
    bool  GetKey(int key);
    bool  GetKeyPress(int key);
    bool  GetKeyRelease(int key);

    Engine();
    ~Engine();
};
```

> `CAudioEngine m_audio` is a **value member**, not a pointer. It is constructed with `Engine` and destroyed with it; `Init()` and `Shutdown()` must still be called explicitly.

---

## Subsystem Members

| Member | Type | Lifetime | Notes |
|---|---|---|---|
| `m_threads` | `ThreadPool*` | Singleton — released by `ShutDown()` | 5 worker threads |
| `m_timer` | `Timer*` | `fnew` / `fdelete` | Static internals; instance triggers `update()` |
| `m_renderer` | `Renderer*` | `fnew` / `fdelete` | Multi-pass OpenGL renderer |
| `m_window` | `WindowClass*` | `fnew` / `fdelete` | GLFW window wrapper |
| `m_input` | `InputReceiver*` | `fnew` / `fdelete` | Static key/mouse state arrays |
| `m_scene` | `Scene::SceneGraph*` | `fnew` / `fdelete` | Entity + component store |
| `m_octree` | `Rendering::Octree*` | `fnew` / `fdelete` | Frustum-culled viewable set |
| `m_particleSystem` | `ParticleSystem*` | `fnew` / `fdelete` | GPU particle pipeline |
| `m_audio` | `CAudioEngine` | Value member | FMOD Studio via PIMPL |

---

## State Enum

```cpp
// Falcon/System/Engine.h  —  namespace gameLoop
enum class STATE { MENU, PAUSEMENU, INGAME };
```

`Engine` stores `m_currentState` internally but does not drive transitions. `Game` owns the state machine and calls the appropriate `Engine::*Update()` method each frame. See [Architecture — Game State Machine](03-Architecture.md#game-state-machine).

> **Note:** A `LoopProperties` struct (per-subsystem enable flags for rendering, physics, input, animation, audio, AI) exists in the header as commented-out code. It was designed to allow selective subsystem gating per state but has not been wired up.

---

## Public API

### Accessors

| Method | Returns | Description |
|---|---|---|
| `GetWindow()` | `WindowClass*` | Raw GLFW window wrapper |
| `GetInput()` | `InputReceiver*` | Input state object |
| `GetUI()` | `UI::UI_Manager*` | UI manager owned by the Renderer |
| `GetKey(int)` | `bool` | `true` while key held |
| `GetKeyPress(int)` | `bool` | `true` on first frame key down |
| `GetKeyRelease(int)` | `bool` | `true` on first frame key up |

All key methods delegate to `InputReceiver`. Use GLFW key codes (e.g., `GLFW_KEY_W`).

### Lifecycle

| Method | Called by | Description |
|---|---|---|
| `Initialize()` | `Game::Init()` | Full subsystem startup; returns `false` on failure |
| `~Engine()` | `Game` destructor | Full subsystem teardown |

---

## Update Paths

Three methods map to the three game states. Each is called once per frame by `Game::Run()`.

### `MenuUpdate()`

```
Renderer::Menu_Update()
Renderer::Menu_Draw()
InputReceiver::Update()
CAudioEngine::Update()
```

Renders the UI canvas and processes input. No physics, scene, or particle updates.

### `IngameUpdate()`

```
Timer::update() → dt
SceneGraph::UpdateScene()
Octree::SetView(camera) + Update()
ParticleSystem::Update(dt, entities)
Renderer::Ingame_Update(dt, entities)
Renderer::Ingame_Draw()
physics::StepPhysics(dt, entities)
InputReceiver::Update()
CarSystem::Update()
CAudioEngine::Update()
```

Full simulation tick. Delta time (`dt`) is derived from the `Timer` and passed through to all time-dependent systems.

The window title is updated each frame to display the current framerate:

```cpp
glfwSetWindowTitle(m_window->GetWindow(), std::to_string(1.0f / dt).c_str());
```

### `PauseUpdate()`

```
InputReceiver::Update()
CAudioEngine::Update()
```

Minimal tick — input is polled so pause-menu navigation works, and FMOD continues its frame tick so music does not stop.

---

## Timer

**Header:** `Falcon/System/Timer.h`

```cpp
class Timer {
    static TimePoint     curr, prev, startTime;
    static TimeDuration  deltaTime;
    static float         dt, tt;
public:
    static void   update();
    static float  GetDeltaTime();
    static float  GetTotalTime();
    static TimeDuration GetCurrentTime();
};
```

`Timer` is entirely static. Calling `update()` records the current `high_resolution_clock` sample and computes the delta against the previous frame. The instance pointer in `Engine` (`m_timer`) exists only to drive `update()` through the `fnew` / `fdelete` lifecycle.

**Usage:**

```cpp
m_timer->update();
float dt = Timer::GetDeltaTime();
```

A separate RAII struct `timer` in `TimeBenchmark.h` records how long a named job takes and logs it via `FL_ENGINE_INFO`. Use it to profile individual tasks:

```cpp
timer t("MyJob");   // starts clock on construction, logs elapsed ms on destruction
```

---

## WindowClass

**Header:** `Falcon/Rendering/WindowHandler.h`

```cpp
class WindowClass {
    int          m_width, m_height, m_bufferWidth, m_bufferHeight;
    const char*  m_title;
    GLFWwindow*  m_gameWindow;
    ThreadPool*  m_threadPool;
public:
    WindowClass(const char* title, int width, int height);
    GLFWwindow*       GetWindow();
    unsigned int      GetWidth();
    unsigned int      GetHeight();
    bool              WindowCloseStatus();   // wraps glfwWindowShouldClose
};
```

The constructor sets the GLFW error callback, calls `Init()` to create the window, and writes `WindowData::windowSize` (a global `glm::vec2`) for any subsystem that needs the viewport dimensions without a `WindowClass` pointer.

`WindowCloseStatus()` is polled by `Game::Run()` as the main loop exit condition.

---

## InputReceiver

**Header:** `Falcon/System/InputReceiver.h`

```cpp
class InputReceiver {
    static bool    m_keyStates[MAX_KEYS];
    static bool    m_prevStates[MAX_KEYS];
    static bool    m_mouseStates[MAX_MOUSE_KEYS];
    static double  m_mouseX, m_mouseY, m_scrollX, m_scrollY;
public:
    InputReceiver(WindowClass* window);
    static void      Init(GLFWwindow*);
    static void      Update();
    static bool      GetKey(int keyValue);
    static bool      GetKeyPress(int keyValue);
    static bool      GetKeyRelease(int keyValue);
    static glm::vec2 GetCursor();
    static glm::vec2 GetScroll();
};
```

`Init()` registers four GLFW callbacks: `key_callback`, `mouse_callback`, `cursor_callback`, and `scroll_callback`. All state is static so it can be queried from anywhere without a pointer.

`Update()` must be called **once per frame** to copy the current key state into the previous-frame buffer — this is what enables edge detection. It runs in all three `Engine::*Update()` paths.

**Edge-detection logic:**

| Method | Condition |
|---|---|
| `GetKey(k)` | `m_keyStates[k] == true` |
| `GetKeyPress(k)` | `m_keyStates[k] && !m_prevStates[k]` |
| `GetKeyRelease(k)` | `!m_keyStates[k] && m_prevStates[k]` |

---

## ThreadPool

**Header:** `Falcon/System/ThreadPool.h`

```cpp
class ThreadPool {
    boost::mutex                 mtx;
    ThreadQueue                  workerQueue;
    GLFWwindow*                  m_window;
    std::atomic<bool>            discard_threadPool;
    static ThreadPool*           mainThreadPool;
    std::vector<boost::thread>   worker_threads;
public:
    static ThreadPool* GetThreadPool(GLFWwindow* window);  // creates instance
    static ThreadPool* GetThreadPool();                    // retrieves instance
    static void ShutDown();
    void execute_task();
    template<typename F>                    void submit(F func);
    template<typename F, typename ...Args>  void submit(F func, Args... as);
};
```

The pool is a **singleton**. `GetThreadPool(window)` creates it the first time; subsequent calls to `GetThreadPool()` return the existing instance. It spawns exactly **5 worker threads** (hardcoded; `hardware_concurrency()` is present but commented out).

Work is submitted via the variadic `submit()` template:

```cpp
ThreadPool::GetThreadPool()->submit(myFunction, arg1, arg2);
```

`ShutDown()` sets `discard_threadPool = true` and joins all threads. Call it before `fmemory::MemoryManagerShutDown()`.

---

## Log

**Header:** `Falcon/System/Log.h`

`Log` wraps **spdlog** and exposes two loggers — engine and client — initialised by `Log::Init()` at the very top of `Engine::Initialize()`. Logging macros are available throughout the codebase:

| Macro | Logger | Level |
|---|---|---|
| `FL_ENGINE_TRACE(...)` | Engine | Trace |
| `FL_ENGINE_INFO(...)` | Engine | Info |
| `FL_ENGINE_WARN(...)` | Engine | Warn |
| `FL_ENGINE_ERROR(...)` | Engine | Error |
| `FL_ENGINE_FATAL(...)` | Engine | Critical |
| `FL_TRACE(...)` | Client | Trace |
| `FL_INFO(...)` | Client | Info |
| `FL_WARN(...)` | Client | Warn |
| `FL_ERROR(...)` | Client | Error |
| `FL_FATAL(...)` | Client | Critical |

All macros accept `{0}`, `{1}` … format placeholders (spdlog / fmtlib style).