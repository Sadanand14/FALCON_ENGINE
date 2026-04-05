# Falcon Engine — Overview

> A C++17 real-time 3D game engine with a car simulation demo, built using OpenGL, NVIDIA PhysX, and FMOD Studio.

---

## Table of Contents

- [Projects](#projects)
- [Technology Stack](#technology-stack)
- [Repository Layout](#repository-layout)
- [Source Tree](#source-tree)
- [Subsystem Map](#subsystem-map)

---

## Projects

The solution is split into two Visual Studio projects with a clean engine/game separation:

| Project | Type | Purpose |
|---|---|---|
| `Falcon` | Static Library (`.lib`) | The engine — all subsystems, no game logic |
| `FALCON_GAME` | Executable (`.exe`) | The game — consumes `Falcon.lib`, owns the game loop and state machine |

`FALCON_GAME` references `Falcon` as a project dependency. All engine code lives in `Falcon`; everything in `FALCON_GAME` is game-specific and replaceable.

---

## Technology Stack

| Concern | Library | Version |
|---|---|---|
| Graphics API | OpenGL via GLAD | 0.1.30 |
| Windowing & Input | GLFW | 3.3 |
| Audio | FMOD Studio | — |
| Physics | NVIDIA PhysX (64-bit) + Vehicle SDK | — |
| Math | GLM | 0.9.9.500 |
| Asset Loading | Assimp | 5.0.0 |
| Animation | ozz-animation | 0.10.0 |
| Containers / Threading | Boost | 1.69 |
| UI | Nuklear | — |
| Scene Serialization | RapidJSON | — |
| Logging | spdlog | — |
| Build System | Premake5 | — |
| Compiler / Toolset | MSVC | v180 (VS 2026), x64 |

> **Note:** All third-party libraries are distributed as a single `vendors.zip` archive.
> See [Getting Started](02-GettingStarted.md) for setup instructions.

---

## Repository Layout

````````
FALCON_ENGINE/
├── Falcon/                   # Engine static library project
│   ├── Falcon/               # Vendor / third-party libraries (extracted from vendors.zip)
│   ├── Audio/
│   ├── Core/
│   ├── Physics/
│   ├── Rendering/
│   ├── System/
│   └── Falcon.vcxproj
├── FALCON_GAME/              # Game executable project
│   ├── Game/
│   ├── Application.cpp
│   └── FALCON_GAME.vcxproj
├── Assets/                   # Runtime assets (extracted from assets.zip)
│   ├── Models/
│   ├── Scenes/
│   ├── Shaders/
│   ├── Sounds/
│   └── Textures/
├── Documentation/            # This folder
├── Setup.ps1                 # Dependency automation script
├── README.md
└── FALCON_ENGINE.sln
````````

---

## Source Tree

### `Falcon` — Engine Library

````````
Falcon/
├── Audio/
│   └── AudioEngine.h/.cpp                  # FMOD Studio wrapper (CAudioEngine)
│
├── Core/
│   ├── AssetManager.h/.cpp                 # Mesh / material / texture / font cache
│   ├── EntityInterface.h                   # Base entity interface
│   ├── EntityManager.h/.cpp                # Entity factory
│   ├── Components/
│   │   ├── AIComponent.h
│   │   ├── AnimationComponent.h
│   │   ├── AudioComponent.h
│   │   ├── BasicComponent.h
│   │   ├── InputComponent.h
│   │   ├── ParticleEmitterComponent.h
│   │   ├── PhysicsComponent.h
│   │   ├── RenderComponent.h
│   │   └── TransformComponent.h
│   ├── Events/
│   │   ├── Event.h                         # Base event type
│   │   ├── EventManager.h/.cpp             # Pub-sub dispatcher
│   │   ├── EventSystem.h                   # Base subscriber interface
│   │   ├── KeyEvents.h
│   │   ├── ParticleEvent.h
│   │   ├── PassToRenderer.h
│   │   ├── PassingMeshEvent.h
│   │   └── RenderEvent.h
│   └── Scene/
│       └── SceneGraph.h/.cpp               # JSON scene loader and node hierarchy
│
├── Physics/
│   ├── Physics.h/.cpp                      # PhysX init / shutdown / step
│   ├── PhysicsSystem.h/.cpp                # Physics event subscriber
│   ├── PXMathUtils.h                       # GLM <-> PhysX math conversions
│   ├── PXUtils.h                           # PhysX helper utilities
│   ├── RayCast.h/.cpp
│   └── vehicle/
│       ├── CarAPI.h/.cpp                   # Public vehicle creation API
│       ├── Vehicle.h/.cpp                  # Car actor and wheel setup
│       ├── VehicleFilterShader.h
│       ├── VehicleInputHandler.h/.cpp
│       ├── VehicleSceneQuery.h/.cpp
│       └── WheelQueryResult.h
│
├── Rendering/
│   ├── Camera.h/.cpp                       # Camera data and movement
│   ├── CameraSystem.h/.cpp                 # Static camera manager
│   ├── Font.h/.cpp
│   ├── OpenGLErrorHandler.h
│   ├── ParticleSystem.h/.cpp
│   ├── BufferDefinitions/
│   │   ├── IndexBuffer.h/.cpp
│   │   ├── VertexArray.h/.cpp
│   │   ├── VertexBuffer.h/.cpp
│   │   └── VertexLayout.h
│   └── PipeLine/
│       ├── Renderer.h/.cpp                 # Multi-pass renderer orchestrator
│       ├── Octree.h/.cpp                   # Frustum culling spatial structure
│       ├── Canvas.h/.cpp                   # UI canvas host
│       ├── UIManager.h/.cpp
│       ├── CanvasItems/
│       │   ├── CanvasItem.h                # Base UI element
│       │   ├── Button.h/.cpp
│       │   └── Image.h/.cpp
│       └── RenderPasses/
│           └── RenderPass.h/.cpp           # Priority-sorted, configurable GL pass
│
└── System/
    ├── Engine.h/.cpp                       # Central orchestrator and game loop
    ├── InputReceiver.h/.cpp                # GLFW input abstraction
    ├── ThreadPool.h/.cpp                   # Boost-backed 5-worker job queue
    ├── Timer.h/.cpp                        # Delta time
    ├── WindowClass.h/.cpp                  # GLFW window wrapper
    └── Memory/
        ├── MemoryManager.h/.cpp            # Pool allocator (fmemory namespace)
        └── STLAllocator.h                  # STL-compatible fmemory adapter
````````

### `FALCON_GAME` — Game Application

````````
FALCON_GAME/
├── Application.cpp                         # Entry point — creates Game, calls Init + loop
├── ExternalLibraries.win.h                 # #pragma comment(lib, ...) for all vendors
└── Game/
    ├── Game.h
    └── Game.cpp                            # State machine, UI setup, input handling
````````

---

## Subsystem Map

````````
FALCON_GAME
└── Game
    └── Engine
        ├── WindowClass          (GLFW window)
        ├── InputReceiver        (GLFW callbacks)
        ├── ThreadPool           (Boost, 5 workers)
        ├── Renderer
        │   ├── RenderPass[]     (priority-sorted GL passes)
        │   ├── Octree           (frustum culling)
        │   ├── ParticleSystem
        │   └── UI_Manager       (Nuklear canvas)
        ├── SceneGraph           (JSON -> SceneNode hierarchy)
        │   └── EntityManager    (Entity + Component factory)
        ├── CameraSystem         (static — shared view matrix)
        ├── CAudioEngine         (FMOD Studio — static API)
        ├── PhysX                (namespace physics::)
        │   └── CarSystem        (vehicle actors)
        ├── AssetManager         (static — mesh/material/texture cache)
        ├── EventManager         (static pub-sub)
        ├── Log                  (static — spdlog wrapper)
        └── fmemory              (namespace — pool allocator)
