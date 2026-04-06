# Contributing

This document covers everything needed to extend or modify the Falcon Engine: project setup, coding standards, engine-specific practices, and the workflows for adding new components, subsystems, and event channels.

---

## Table of Contents

- [Repository](#repository)
- [Project Setup](#project-setup)
- [Build Configurations](#build-configurations)
- [Vendor Library Versions](#vendor-library-versions)
- [Coding Style](#coding-style)
- [Engine Practices](#engine-practices)
- [Adding a Component](#adding-a-component)
- [Adding an EventSystem](#adding-an-eventsystem)
- [Adding a New Event Category](#adding-a-new-event-category)
- [Updating premake5.lua](#updating-premake5lua)
- [Continuous Integration](#continuous-integration)

---

## Repository

| Item | Value |
|---|---|
| Remote | `https://github.com/Sadanand14/FALCON_ENGINE` |
| Primary branch | `master` |
| Engine static library | `Falcon.lib` (project: `Falcon`) |
| Game executable | `FALCON_GAME.exe` (project: `FALCON_GAME`) |
| Entry point | `FALCON_GAME/Application.cpp` → `main()` |

---

## Project Setup

### Windows

| Step | Action |
|---|---|
| 1 | Clone the repository |
| 2 | Download the Windows vendor archive and unzip it under `FALCON_ENGINE\Falcon\Falcon\` |
| 3 | Download the Windows assets archive and unzip it under `FALCON_ENGINE\Falcon\` |
| 4 | Run `GenProject.bat` from the repository root to generate Visual Studio project files via Premake5 |
| 5 | Open the generated solution in Visual Studio, set configuration to **Debug \| x64**, and build |

### Linux

| Step | Action |
|---|---|
| 1 | Run `sudo apt-get update` then install: `python python-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev` |
| 2 | Clone the repository |
| 3 | Download the Linux vendor tarball and unzip under `FALCON_ENGINE\Falcon\Falcon\` |
| 4 | Download the Linux assets tarball and unzip under `FALCON_ENGINE\Falcon\build\Debug\` |
| 5 | Run `GenProject.sh` to generate makefiles |
| 6 | Run `make` to build; `make clean` to wipe the build directory |

### Automated Vendor / Asset Download

An automated setup script is documented in `tools/Readme.md` and handles the vendor and asset download steps above.

---

## Build Configurations

| Configuration | Preprocessor define | Behaviour |
|---|---|---|
| **Debug \| x64** | `BUILD_DEBUG_MODE` | All `FL_ENGINE_*` / `FL_GAME_*` log macros active; rapidjson uses `PrettyWriter`; PVD connection compiled in |
| **Release \| x64** | *(none)* | All log macros are no-ops; rapidjson uses compact `Writer`; full optimisation |

> Always run and test in **Debug \| x64** first. The PhysX Visual Debugger connects automatically in debug — see `07-Physics.md`.

---

## Vendor Library Versions

| Library | Version | Purpose |
|---|---|---|
| Assimp | 5.0.0 | Mesh import (FBX, OBJ, glTF) |
| Boost | 1.69 | Threads, containers, smart pointers, circular buffer |
| GLAD | 0.1.30 | OpenGL function loader |
| GLFW | 3.3 | Window and input |
| GLM | 0.9.9.500 | Mathematics |
| OZZ | 0.10.0 | Animation runtime |
| FMOD Studio | (vendor) | Audio |
| NVIDIA PhysX | 64-bit (vendor) | Physics and vehicle simulation |
| spdlog | (vendor) | Logging backend |
| rapidjson | (vendor) | JSON serialisation |
| Nuklear | (vendor) | Immediate-mode UI |
| stb_dds | (vendor) | DDS texture loading |

Do not upgrade a vendor library without verifying the full build on both Windows and Linux. Vendor headers live under `Falcon\Falcon\vendor\`.

---

## Coding Style

These rules are taken directly from the project's own coding standard.

### Naming

| Entity | Convention | Example |
|---|---|---|
| Class, struct, function, method | PascalCase | `RenderEventSystem`, `GetMesh()` |
| Variable, parameter | camelCase | `meshPath`, `m_entityList` |
| Member variable | `m_` prefix + camelCase | `m_updateFlag`, `m_threadPool` |
| Macro | ALL_CAPS with underscores | `FL_ENGINE_ERROR`, `INIT_MEMORY_MANAGER` |
| Namespace | lowercase | `physics`, `fmemory`, `Scene` |

### Formatting

| Rule | Detail |
|---|---|
| Indentation | Tabs — not spaces — except when aligning continuation arguments |
| Brace placement | Opening brace on its own line for all blocks |
| Access specifiers | `private` → `protected` → `public` in both header and source file |
| Long call sites | Break after the opening parenthesis; align continuation arguments |

### Headers

Use `#ifndef` / `#define` / `#endif` include guards — **never** `#pragma once`. The guard symbol is the filename in ALL_CAPS with underscores and a trailing `_H_` suffix — for example, a file named `MySystem.h` uses `MY_SYSTEM_H_`.

### Classes

Initialise members in constructor initialiser lists rather than in the constructor body. Mark all single-expression accessors both `inline` and `const`. List members in the same order in the header and in the `.cpp` file.

### Types

Use the fixed-size numeric aliases from `Types.h` — never bare built-in integer types:

| Alias | Underlying | Replaces |
|---|---|---|
| `u8` | `uint8_t` | `unsigned char` |
| `u16` | `uint16_t` | `unsigned short` |
| `u32` | `uint32_t` | `unsigned int` |
| `u64` | `uint64_t` | `unsigned long long` |
| `i32` | `int32_t` | `int` |

---

## Engine Practices

### Memory

| Rule | Detail |
|---|---|
| Allocate engine objects | `fmemory::fnew<T>(args...)` |
| Release engine objects | `fmemory::fdelete<T>(ptr)` |
| Release engine arrays | `fmemory::fdelete<T>(ptr, count)` |
| STL containers in engine code | `boost::container::vector<T, fmemory::STLAllocator<T>>` |
| Standard `new` / `delete` | Acceptable only in the `FALCON_GAME` application layer |

`INIT_MEMORY_MANAGER` must be the **first** call in `Engine::Initialize()`. `MEMORY_MANAGER_SHUT_DOWN` must be the **last** call in `Engine::~Engine()`. Any subsystem that holds `fmemory`-allocated objects must release them before the engine destructor returns.

### Logging

Use the engine macros — never call spdlog directly. All macros compile to nothing in release builds, so it costs nothing to leave diagnostic calls in place.

| Situation | Macro |
|---|---|
| Status or progress | `FL_ENGINE_INFO(...)` |
| Verbose per-frame data | `FL_ENGINE_TRACE(...)` |
| Unexpected but non-fatal | `FL_ENGINE_WARN(...)` |
| Recoverable error | `FL_ENGINE_ERROR(...)` |
| Unrecoverable error | `FL_ENGINE_FATAL(...)` |

Game-layer code uses the matching `FL_GAME_*` variants. Both families accept the same `{0}`, `{1}` positional fmt syntax.

### Raw I/O

Do not perform file or network I/O outside the designated subsystems: `AssetManager` for assets, `SceneGraph` for scene JSON, `CAudioEngine` for FMOD banks. Do not introduce platform-specific code — the engine maintains a Linux build.

---

## Adding a Component

| Step | What to do |
|---|---|
| 1 — Header | Create `Falcon/Core/Components/MyComponent.h`. The struct must inherit `BasicComponent` and use a proper `#ifndef` guard. |
| 2 — Entity member | Add `#include <Components/MyComponent.h>` to `EntityInterface.h` and declare `MyComponent* m_myC = nullptr;` in `Entity`'s private section. |
| 3 — Specialisations | Add `AddComponent<MyComponent>()` and `GetComponent<MyComponent>()` explicit specialisations in `EntityInterface.h` following the identical pattern used by every existing component. |
| 4 — Destructor | Add `if (m_myC) fmemory::fdelete<>(m_myC);` to `Entity::~Entity()`. |
| 5 — JSON (optional) | If the component has JSON-driven fields, add parsing logic in `EntityManager::CreateEntity()` inside `SceneGraph.cpp`. |
| 6 — Premake | If a new source directory was created, add it to `premake5.lua` and regenerate — see [Updating premake5.lua](#updating-premake5lua). |

---

## Adding an EventSystem

| Step | What to do |
|---|---|
| 1 — Constructor | In the subclass constructor, assign `m_threadPool = ThreadPool::GetThreadPool()`, push each desired `EventsCategory` into `subscribedList`, then call `SubscribeToEvents()`. |
| 2 — SubscribeToEvents | Iterate `subscribedList` and call `EventManager::SubscribeToEvent(this, category)` for each entry. |
| 3 — ProcessEvents | Drain `eventQueue` each frame — pop each `boost::shared_ptr<Event>` from the front of the circular buffer and dispatch on `event->CheckCategory()`. |
| 4 — Wiring | Instantiate the system during the owning subsystem's initialisation. Call `ProcessEvents()` from that subsystem's per-frame update method. |

If the system is a singleton, follow the `RenderEventSystem` / `ParticleEventSystem` pattern: a private constructor, a static `m_instance` pointer, a static `GetInstance()` factory, and a static `ShutDown()` that deletes the instance.

---

## Adding a New Event Category

| Step | What to do |
|---|---|
| 1 — Enum | Add `EVENT_MY_CATEGORY = BIT(n)` to the `EventsCategory` enum in `Falcon/Core/Events/Event.h`, where `n` is the next unused bit index. Currently the highest used index is 8 (`EVENT_CAR_AI_INPUT`), so the next is 9. |
| 2 — Event subclass | Create a struct inheriting `Event` that passes the new category to the base constructor and adds whatever payload fields are needed. |
| 3 — Subscribe | Register an `EventSystem` subclass for the new category as described in [Adding an EventSystem](#adding-an-eventsystem). |
| 4 — Publish | Dispatch via `EventManager::PushEvent(boost::make_shared<MyEvent>(...), EVENT_MY_CATEGORY)` from wherever the trigger occurs. |

---

## Updating premake5.lua

Premake is the **authoritative** project configuration. The `.vcxproj` files and makefiles are generated outputs — never edit them by hand.

| Action | Location in `premake5.lua` |
|---|---|
| New source directory | Add the path glob to the relevant `files { }` block |
| New include directory | Add to `includedirs { }` in the affected project block |
| New static library link | Add to `links { }` |
| New preprocessor define | Add to `defines { }` under the appropriate `filter` block |

After editing `premake5.lua`, regenerate:

| Platform | Command |
|---|---|
| Windows | `GenProject.bat` |
| Linux | `GenProject.sh` |

Commit the updated `premake5.lua` alongside your source changes. Do **not** commit regenerated `.vcxproj` or `Makefile` files — they are in `.gitignore` and must be regenerated locally by each contributor.

---

## Continuous Integration

A CI pipeline on **Azure DevOps** runs on every push to `master` and validates the Windows x64 build.

| Item | Value |
|---|---|
| Build dashboard | `https://dev.azure.com/san2889/san2889/_build` |
| Trigger | Every push to `master` |
| Badge | `README.md` reflects current `master` build state |

If the `master` build is broken, fix it before merging further changes.

