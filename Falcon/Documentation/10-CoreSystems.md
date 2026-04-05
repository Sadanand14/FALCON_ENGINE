# Core Systems

This document covers the four foundational utility systems that every other engine subsystem depends on: the **AssetManager** asset cache, the **fmemory** pool allocator, the **ThreadPool** job system, and the **Log** diagnostic wrapper.

---

## Table of Contents

- [AssetManager](#assetmanager)
- [fmemory Allocator](#fmemory-allocator)
- [ThreadPool](#threadpool)
- [Log](#log)

---

## AssetManager

**Header:** `Falcon/Core/AssetManager.h`

`AssetManager` is a purely static class — all methods and data are `static`, and no instance is ever created. It owns three caches that map file path strings to loaded objects, all protected by a shared `boost::mutex`.

### Static Caches

| Cache | Type | Contents |
|---|---|---|
| `m_meshes` | `boost::unordered_map<string, Mesh*>` | All loaded geometry |
| `m_materials` | `boost::unordered_map<string, Material*>` | All loaded PBR material sets |
| `m_fonts` | `boost::unordered_map<string, Font*>` | All loaded Nuklear fonts |

IBL pre-computation uses three additional static members — `m_cubeShader`, `m_irradianceShader`, `m_prefilterShader`, and `m_cubeMesh` — that are allocated once and reused for every sky material load.

### Get Methods (cached, load-on-miss)

| Method | Returns | Description |
|---|---|---|
| `GetMesh(path)` | `Mesh*` | Returns cached mesh or calls `LoadModel()` |
| `GetMaterial(path)` | `Material*` | Returns cached material or calls `LoadMaterial()` |
| `GetSkyMaterial(path)` | `Material*` | Returns cached sky material or calls `LoadSkyMaterial()` |
| `GetSky(path)` | `Mesh*` | Returns cached sky mesh or loads it |
| `GetFont(path)` | `Font*` | Returns cached font or calls `LoadFont()` |

### Load Methods (bypass cache)

| Method | Returns | Description |
|---|---|---|
| `LoadModel(path)` | `Mesh*` | Imports geometry via **Assimp** — calls `ProcessNode()` and `ProcessMesh()` recursively |
| `LoadTerrain(path)` | `Mesh*` | Loads a terrain mesh from the given path |
| `LoadTexture(path, channels)` | `u32` (OpenGL handle) | Loads a **DDS** texture via `stb_dds`; `channels` selects the target format |
| `LoadMaterial(path)` | `Material*` | Reads a JSON material descriptor via **rapidjson** and loads each texture channel |
| `LoadSkyMaterial(path)` | `Material*` | Loads an HDR equirectangular map and runs IBL pre-computation (`HDRtoCubemap`) to produce a cubemap, irradiance map, and pre-filter map |
| `LoadFont(path)` | `Font*` | Loads a font file for use with **Nuklear** |
| `CreateDefaultFont()` | `void` | Registers the Nuklear built-in default font |

### Utility

| Method | Description |
|---|---|
| `Clean()` | Releases all cached `Mesh*`, `Material*`, and `Font*` objects; called during shutdown |

### JSON Serialisation Mode

In `BUILD_DEBUG_MODE` builds, rapidjson uses `PrettyWriter` for human-readable output. Release builds use the standard `Writer` for compact output. The active alias is `Writer` in both cases — only the underlying type changes.

### Third-Party Libraries Used

| Library | Purpose |
|---|---|
| **Assimp** | FBX / OBJ / glTF mesh import; post-processing flags apply tangent generation, triangulation, and index optimisation |
| **stb_dds** | DDS compressed texture loading |
| **rapidjson** | JSON material descriptor parsing and scene serialisation |
| **Nuklear** | Immediate-mode UI; font baking via `NK_INCLUDE_FONT_BAKING` |

---

## fmemory Allocator

**Header:** `Falcon/System/Memory/fmemory.h`

`fmemory` is a namespace — not a class — that exposes a pool-based memory system to replace `new`/`delete` throughout the engine. All engine-owned heap allocations use `fmemory::fnew<T>()` and `fmemory::fdelete()`.

### Lifecycle

| Macro / Function | Called from | Description |
|---|---|---|
| `INIT_MEMORY_MANAGER` → `fmemory::MemoryManagerInit()` | `Engine::Initialize()`, step 1 | Must be called before any `fnew`/`fdelete` usage |
| `MEMORY_MANAGER_SHUT_DOWN` → `fmemory::MeoryManagerShutDown()` | `Engine::~Engine()`, last step | Flushes and releases all pools; must run after every other subsystem has freed its memory |

> Note: the shutdown function name contains a typo in the source — `MeoryManagerShutDown` (missing the 'm' in Memory). Use the macro `MEMORY_MANAGER_SHUT_DOWN` to avoid depending on the misspelled name directly.

### Core Allocation Functions

| Function | Description |
|---|---|
| `fmemory::Allocate(size)` | Returns a block from the pool that fits `size`; falls back to system `malloc` if no pool can accommodate it |
| `fmemory::Free(ptr, size)` | Returns `ptr` to the pool whose block size matches `size`; `size` must match the original allocation |
| `fmemory::AllocateOnStack(size)` | Allocates from the stack allocator — for short-lived, frame-scoped data |
| `fmemory::FlushStack()` | Resets the stack allocator back to its base pointer |

### Object API

| Function | Description |
|---|---|
| `fmemory::fnew<T>(args...)` | Calls `fmemory::Allocate(sizeof(T))` then placement-`new T(args...)`; returns `T*` or `nullptr` on failure |
| `fmemory::fnew_arr<T>(count, args...)` | Allocates a contiguous block for `count` objects and placement-constructs each element |
| `fmemory::fdelete<T>(ptr)` | Calls `ptr->~T()` then `fmemory::Free(ptr, sizeof(T))` |
| `fmemory::fdelete<T>(ptr, count)` | Destructs each element then frees the array block via `fmemory::Free(ptr, sizeof(T) * count)` |

All four functions catch `std::exception` internally and log `FL_ENGINE_ERROR` on failure — they never throw.

### STL Integration

`fmemory::STLAllocator<T>` is a stateless allocator that satisfies the `std::allocator` concept, routing `allocate()` / `deallocate()` through `fmemory::Allocate` and `fmemory::Free`. It is used as the second template argument to all `boost::container::vector` instances in the engine:

| Typedef | Used for |
|---|---|
| `entityVector` | `boost::container::vector<Entity*, STLAllocator<Entity*>>` |
| `nodeVector` | `boost::container::vector<SceneNode*, STLAllocator<SceneNode*>>` |
| `IDVector` | `boost::container::vector<unsigned short int, STLAllocator<unsigned short int>>` |

### Sub-headers

| Header | Contents |
|---|---|
| `MemoryInterface.h` | `Allocate`, `Free`, `MemoryManagerInit`, `MeoryManagerShutDown`, `AllocateOnStack`, `FlushStack` |
| `fnew.h` | `fnew`, `fnew_arr`, `fdelete` templates |
| `STLMemoryInterface.h` | `STLAllocator<T>` |
| `PoolAllocator.hpp` | Pool allocator implementation |
| `StackAllocator.h` | Stack allocator implementation |

---

## ThreadPool

**Header:** `Falcon/System/ThreadPool.h`

`ThreadPool` is a **singleton** that owns 5 persistent worker threads. All engine subsystems that submit background work hold a `ThreadPool*` obtained via `GetThreadPool()`.

### Types

| Typedef | Underlying type | Description |
|---|---|---|
| `void_function` | `boost::function<void()>` | Unit of work accepted by the pool |
| `ThreadQueue` | `std::queue<void_function>` | FIFO work queue protected by `boost::mutex` |

### Singleton API

| Method | Description |
|---|---|
| `GetThreadPool(GLFWwindow* window)` | Creates the singleton on first call; `window` is stored to allow worker threads to call GLFW context operations |
| `GetThreadPool()` | Returns the existing singleton; asserts if called before the first overload |
| `ShutDown()` | Sets `discard_threadPool = true`; joins all 5 worker threads; frees the singleton |

### Work Submission

| Method | Description |
|---|---|
| `submit(func)` | Locks `mtx`, pushes `func` cast to `void_function`, unlocks |
| `submit(func, args...)` | Binds `args` into a `void_function` via a lambda, then delegates to the single-argument overload |

Both overloads are function templates — any callable (function pointer, lambda, `std::bind` result) is accepted.

### Worker Loop

`execute_task()` is the entry point for each of the 5 `boost::thread` workers. Each worker loops continuously, locking `mtx` to dequeue one job at a time and executing it, until `discard_threadPool` is set to `true`.

### Lifecycle

| Phase | Call |
|---|---|
| Startup | `ThreadPool::GetThreadPool(window)` — called in `Engine::Initialize()` |
| Per-frame | `submit()` calls from `Renderer`, `ParticleSystem`, etc. |
| Shutdown | `ThreadPool::ShutDown()` — called in `Engine::~Engine()` before memory shutdown |

---

## Log

**Header:** `Falcon/System/Log.h`

`Log` is a thin static wrapper over **spdlog** that provides two named loggers — one for engine-internal messages and one for game-layer messages — and exposes them through convenience macros.

### Initialisation

`Log::Init()` is called during `Engine::Initialize()` (step 2, immediately after memory). It creates two `spdlog::stdout_color_mt` loggers:

| Logger | Accessor | Purpose |
|---|---|---|
| Engine logger | `Log::GetEngineLogger()` | Internal engine diagnostics |
| Game logger | `Log::GetGameLogger()` | Application / gameplay messages |

### Macros

All macros are **no-ops in release builds** (`BUILD_DEBUG_MODE` not defined). In debug builds they forward to the corresponding spdlog level.

**Engine macros:**

| Macro | spdlog level | Typical use |
|---|---|---|
| `FL_ENGINE_INFO(...)` | `info` | General status messages |
| `FL_ENGINE_TRACE(...)` | `trace` | Verbose per-frame diagnostics |
| `FL_ENGINE_WARN(...)` | `warn` | Non-fatal unexpected conditions |
| `FL_ENGINE_ERROR(...)` | `error` | Recoverable errors (duplicate component, alloc failure, etc.) |
| `FL_ENGINE_FATAL(...)` | `fatal` | Unrecoverable conditions |

**Game macros** (same severity levels, game logger):

| Macro |
|---|
| `FL_GAME_INFO(...)` |
| `FL_GAME_TRACE(...)` |
| `FL_GAME_WARN(...)` |
| `FL_GAME_ERROR(...)` |
| `FL_GAME_FATAL(...)` |

All macros accept a printf-style format string followed by variadic arguments — the same `{0}`, `{1}` positional syntax used by spdlog's fmt backend.