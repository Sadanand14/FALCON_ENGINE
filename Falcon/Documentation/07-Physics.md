# Physics

The physics subsystem wraps **NVIDIA PhysX** (64-bit) through a set of free functions in the `physics::` namespace and a higher-level `CarSystem` singleton for vehicle simulation. `PhysicsComponent` bridges the ECS and the PhysX SDK per entity.

---

## Table of Contents

- [Overview](#overview)
- [Initialization and Shutdown](#initialization-and-shutdown)
- [Per-Frame Step](#per-frame-step)
- [Core Free Functions](#core-free-functions)
- [Rigidbody API](#rigidbody-api)
- [Collider Shapes](#collider-shapes)
- [PhysicsComponent](#physicscomponent)
- [Vehicle System](#vehicle-system)
- [Vehicle API](#vehicle-api)
- [Drive Modes](#drive-modes)
- [PhysX Visual Debugger](#physx-visual-debugger)

---

## Overview

Physics and vehicle simulation run **after** rendering each frame. `StepPhysics` advances the PhysX scene and writes resulting poses back into each entity's `TransformComponent`. `CarSystem::Update()` applies player and AI inputs to vehicle actors each frame.

Per-frame flow:

| Step | Call | Description |
|---|---|---|
| 1 | `Engine::IngameUpdate()` | Orchestrates all per-frame work |
| 2 | `physics::StepPhysics(dt, entities)` | Calls `PxScene::simulate(dt)` then `fetchResults()` |
| 3 | `fetchResults()` | Blocks until PhysX step completes |
| 4 | Transform sync | Copies each actor's `PxTransform` back to `TransformComponent` |
| 5 | `CarSystem::Update()` | Queues vehicle inputs — consumed on next frame's step |

---

## Initialization and Shutdown

**Init** (`Engine::Initialize()` — steps 6–7):

| Call | What it creates |
|---|---|
| `physics::InitPhysX()` | `PxFoundation`, `PxPhysics`, `PxCooking`, `PxScene`, PVD connection |
| `CarSystem::Initialize()` | Vehicle SDK, `CarEventSystem`, user and AI car arrays |

**Shutdown** (`Engine::~Engine()` — steps 3–4):

| Call | Order |
|---|---|
| `CarSystem::ShutDown()` | First — car actors must be released before the scene |
| `physics::ShutdownPhysX()` | Second — releases `PxScene`, `PxPhysics`, `PxFoundation` |

> PhysX must shut down **before** `fmemory::MemoryManagerShutDown()` and **after** `CarSystem::ShutDown()`, since car actors hold references into the PhysX scene.

`PX_SUPPORT_PVD` is defined globally — the PhysX Visual Debugger connection is always compiled in. See [PhysX Visual Debugger](#physx-visual-debugger).

---

## Per-Frame Step

Called from `Engine::IngameUpdate()` after rendering via `physics::StepPhysics(dt, m_scene->GetEntities(), m_scene->GetEntities()->size())`.

`StepPhysics` calls `PxScene::simulate(dt)` followed by `fetchResults(true)`, then iterates every entity with a `PhysicsComponent` and copies the actor's `PxTransform` back into the entity's `TransformComponent`.

The full per-frame order in `IngameUpdate()`:

| Order | Call | Notes |
|---|---|---|
| 1 | `Renderer::Ingame_Update / Ingame_Draw` | Render first |
| 2 | `physics::StepPhysics(dt, entities)` | Simulate + sync transforms |
| 3 | `InputReceiver::Update()` | Poll GLFW input |
| 4 | `CarSystem::Update()` | Queue vehicle inputs for next frame |
| 5 | `CAudioEngine::Update()` | FMOD tick |

> Vehicle inputs queued by `CarSystem::Update()` on frame N are consumed during `StepPhysics` on frame N+1.

---

## Core Free Functions

**Header:** `Falcon/Physics/Physics.h` — `namespace physics`

### Lifecycle

| Function | Description |
|---|---|
| `InitPhysX()` | Creates `PxFoundation`, `PxPhysics`, `PxCooking`, `PxScene`, and PVD connection |
| `CreatePhysicsScene()` | Allocates and configures the `PxScene` |
| `StepPhysics(dt, entities, count)` | Advances simulation one step and syncs transforms |
| `ShutdownPhysX()` | Releases all PhysX objects via the `PX_RELEASE` macro |

### Getters

| Function | Returns | Description |
|---|---|---|
| `GetPhysics()` | `PxPhysics*` | Main SDK object |
| `GetPhysicsScene()` | `PxScene*` | Active simulation scene |
| `GetCooking()` | `PxCooking*` | Mesh cooking for convex / triangle mesh baking |
| `GetAllocator()` | `PxDefaultAllocator` | Default PhysX allocator |
| `GetDefaultMaterial()` | `PxMaterial*` | Default friction / restitution material |

`PX_RELEASE(x)` calls `x->release()` and sets `x = NULL`. It is used throughout shutdown to safely release every PhysX object in dependency order.

---

## Rigidbody API

| Function | Returns | Description |
|---|---|---|
| `CreateStaticRigidActor(transform, collider)` | `PxRigidStatic*` | Immovable actor with a pre-built shape |
| `CreateDynamicRigidActor(transform, collider)` | `PxRigidDynamic*` | Simulated actor with a pre-built shape |
| `CreateDynamicRigidActor(isCCDEnabled)` | `PxRigidDynamic*` | Empty dynamic actor — attach shape separately |
| `CreatePlane()` | `PxRigidStatic*` | Infinite static ground plane |
| `ReleaseCollider(actor)` | `void` | Detaches actor from scene and releases it |
| `ReleaseResource<T>(ref)` | `void` | Generic `PX_RELEASE` wrapper (template) |

**Body type enum** — `namespace physics`:

| Value | Meaning |
|---|---|
| `PhysicsBodyType::ESTATIC_BODY` | Immovable geometry |
| `PhysicsBodyType::EDYNAMIC_BODY` | Simulated rigid body |
| `PhysicsBodyType::EPLANE` | Infinite static ground plane |

---

## Collider Shapes

All shape creation functions return `PxShape*` and use `GetPhysics()` and `GetCooking()` internally.

| Function | Shape | Parameters |
|---|---|---|
| `GetBoxCollider(hX, hY, hZ)` | Box | Half-extents per axis |
| `GetSphereCollider(radius)` | Sphere | Radius |
| `GetCapsuleCollider(radius, halfH)` | Capsule | Radius + half-height along Y |
| `GetMeshCollider(verts, stride, count, scale)` | Convex mesh | Vertex array; cooked by `PxCooking` |
| `GetMeshColliderWithTriangleMeshes(verts, indices, ...)` | Triangle mesh | Vertex + index arrays; static geometry only |

Convex and triangle mesh colliders share vertex and index data with the render mesh. `Mesh::GetVertexPositionsArray()` and `Mesh::GetindicesArray()` supply that data directly to the cooking functions, so the physics and render shapes are always in sync.

**Collider type enum** — `namespace physics`:

| Value | Shape |
|---|---|
| `PhysicsCollider::ESPHERE_COLLIDER` | Sphere |
| `PhysicsCollider::EBOX_COLLIDER` | Box |
| `PhysicsCollider::ECAPSULE_COLLIDER` | Capsule |

---

## PhysicsComponent

**Header:** `Falcon/Core/Components/PhysicsComponent.h`

Convenience type aliases defined at file scope:

| Alias | PhysX type |
|---|---|
| `Rigidbody` | `physx::PxRigidActor` |
| `RigidbodyDynamic` | `physx::PxRigidDynamic` |
| `RigidbodyStatic` | `physx::PxRigidStatic` |
| `Collider` | `physx::PxShape` |
| `ConvexMesh` | `physx::PxConvexMesh` |

`PhysicsComponent` inherits from `BasicComponent` and holds one `Rigidbody*` actor and one `Collider*` shape. The setter methods are:

| Method | Description |
|---|---|
| `SetBoxCollider(hX, hY, hZ)` | Creates a box shape and assigns to `m_collider` |
| `SetSphereCollider(radius)` | Creates a sphere shape and assigns to `m_collider` |
| `SetCapsuleCollider(radius, halfH)` | Creates a capsule shape and assigns to `m_collider` |
| `SetConvexMeshCollider(...)` | Creates a cooked convex mesh shape |
| `SetMeshCollider(...)` | Creates a cooked triangle mesh shape |

Each setter checks whether `m_collider` is already assigned and logs `FL_ENGINE_ERROR` if so — a component may hold only one collider at a time. The `PxRigidActor` is created separately and attached during `SceneGraph` JSON deserialization.

---

## Vehicle System

**Header:** `Falcon/Physics/CarSystem.h`

`CarSystem` is a **static singleton** that owns two car arrays — user-controlled and AI — and a `CarEventSystem` for event-driven input dispatch.

| Method | Description |
|---|---|
| `Initialize()` | Initialises the vehicle SDK, event system, and car arrays |
| `ShutDown()` | Releases all car actors and vehicle SDK resources |
| `Update()` | Advances all active vehicles using queued input data |
| `AddCar(car, isUserCar)` | Registers a `CarStruct*`; `isUserCar` routes it to the correct array |
| `ApplyInputToUserCar(driveMode)` | Queues a `DriveMode` input for the current user car |
| `GetUserCars()` | Returns `CarArr*` of player-controlled cars |
| `GetAICars()` | Returns `CarArr*` of AI-controlled cars |

`Update()` calls `physics::vehicle::IncrementDrivingMode()` on each active car, consuming queued `DriveMode` inputs and stepping the PhysX Vehicle SDK. Call `AddCar()` after constructing a `CarStruct` and before the first `Update()`.

---

## Vehicle API

**Header:** `Falcon/Physics/vehicle/CarAPI.h` — `namespace physics::vehicle`

| Function | Description |
|---|---|
| `CreateVehicleDescriptionObject(car)` | Fills wheel and drive simulation data on the `Car` struct |
| `CreateVehicle4W(car, vehActor)` | Creates `PxVehicleDrive4W` from the description |
| `SetupVehicleActorData(car, vehActor)` | Attaches wheel shapes and user data to the PhysX actor |
| `ConfigureCarData(vehicle, actorUD, shapeUD)` | Assigns `ActorUserData` and `ShapeUserData` pointers |
| `IncrementDrivingMode(car, inputData, dt)` | Applies smoothed input and advances vehicle state |
| `CustomizeVehicleToLengthScale(scale, ...)` | Rescales all vehicle parameters by a length scale factor |
| `ScaleAckermanData(scale, driveSimData)` | Applies length scale to Ackermann steering geometry |

**Steer-vs-speed table** (baked into `Vehicle.h`):

| Forward speed (m/s) | Max steer fraction |
|---|---|
| 0–5 | 0.75 |
| 5–30 | 0.75 → 0.125 (interpolated) |
| 30–120 | 0.125 → 0.1 (interpolated) |

**Key input smoothing** (`gKeySmoothingData`):

| Input axis | Rise rate | Fall rate |
|---|---|---|
| Accelerate / Brake / Handbrake | 6.0 | 10.0 |
| Steer left / right | 2.5 | 5.0 |

**Analogue pad smoothing** (`gPadSmoothingData`) uses significantly lower rates (0.025–0.10 rise, 0.010–0.100 fall) for smoother analogue response.

---

## Drive Modes

Input is applied via `CarSystem::ApplyInputToUserCar(DriveMode)` or directly through `physics::vehicle` functions on `PxVehicleDrive4WRawInputData`:

| Function | Action |
|---|---|
| `StartAccelerateForwardsMode` | Throttle forward |
| `StartAccelerateReverseMode` | Throttle reverse |
| `StartBrakeMode` | Apply brakes |
| `StartTurnHardLeftMode` | Full left steer |
| `StartTurnHardRightMode` | Full right steer |
| `StartHandbrakeTurnLeftMode` | Handbrake + left steer |
| `StartHandbrakeTurnRightMode` | Handbrake + right steer |
| `ReleaseAllControls` | Zero all inputs |

One surface type is currently defined: `SURFACE_TYPE_TARMAC`. Additional types (grass, gravel, etc.) can be added to the `SurfaceTypes` enum in `Vehicle.h` and wired into the friction pair table.

---

## PhysX Visual Debugger

`PX_SUPPORT_PVD` is defined globally in `Physics.h`, which compiles in PVD support unconditionally.

**To connect:**

1. Launch **NVIDIA PhysX Visual Debugger** on the same machine.
2. Run the engine in **Debug | x64** configuration.
3. PhysX will attempt a TCP connection to `127.0.0.1:5425` automatically during `InitPhysX()`.

PVD provides real-time inspection of rigid body poses, contact manifolds, joint constraints, and vehicle suspension travel. It is the primary diagnostic tool for any physics anomaly that is not visible in the game viewport.

