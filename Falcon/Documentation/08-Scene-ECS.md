# Scene & ECS

The scene system combines a **tree-structured SceneGraph** with a lightweight **Entity-Component** model. Every game object is an `Entity` that owns a fixed `Transform` plus any number of optional typed components. `SceneGraph` owns the full entity hierarchy for one scene, loads it from JSON via rapidjson, and exposes the partitioned entity lists consumed by rendering, physics, and the Octree each frame.

---

## Table of Contents

- [Overview](#overview)
- [Entity Class](#entity-class)
- [Component System](#component-system)
- [Transform Component](#transform-component)
- [SceneNode Hierarchy](#scenenode-hierarchy)
- [SceneGraph](#scenegraph)
- [EntityManager](#entitymanager)
- [JSON Scene Format](#json-scene-format)
- [Scene Lifecycle](#scene-lifecycle)

---

## Overview

| Layer | Type | Responsibility |
|---|---|---|
| `Entity` | Game object | Owns one `Transform*` + up to 8 optional components |
| `SceneNode` | Tree node | Wraps an `Entity*`, tracks parent/children, propagates world matrices |
| `SceneGraph` | Scene owner | Loads JSON, builds the node tree, maintains partitioned entity vectors |
| `EntityManager` | Static factory | Creates entities from JSON templates; saves/closes the scene |

Per-frame data flow through the scene:

| Step | Call | Consumer |
|---|---|---|
| 1 | `SceneGraph::UpdateScene()` | Propagates parent matrices down the SceneNode tree |
| 2 | `GetRenderables()` | `Renderer` — entities with `RenderComponent` |
| 3 | `GetOctreeEntities()` | `Octree` — transform-dirty renderables only |
| 4 | `GetEntities()` | `physics::StepPhysics` — all entities for PhysX transform sync |

---

## Entity Class

**Header:** `Falcon/Core/EntityInterface.h`

`Entity` is the fundamental game object. It always allocates a `Transform` in its constructor via `fmemory::fnew<Transform>()`. All other components are `nullptr` until explicitly added.

| Method | Description |
|---|---|
| `Entity()` | Creates entity with default `Transform` at origin |
| `Entity(pos, rot, scale)` | Creates entity with `Transform` initialised to the supplied values |
| `~Entity()` | Calls `fmemory::fdelete<>()` on every non-null component |
| `GetTransform()` | Returns `Transform*` — always valid for the lifetime of the entity |
| `AddComponent<T>()` | Allocates `T` via `fmemory::fnew<T>()` and stores it; logs `FL_ENGINE_ERROR` if the slot is already occupied |
| `GetComponent<T>()` | Returns the typed component pointer, or `nullptr` if not present |

`AddComponent<T>()` and `GetComponent<T>()` are fully specialised for each of the 8 optional component types. Adding a component that already exists is a non-fatal error — the existing component is kept and the request is ignored.

---

## Component System

All component types inherit `BasicComponent` (an empty base struct). Every component is heap-allocated by `Entity::AddComponent<T>()` using `fmemory::fnew<T>()` and released by the entity destructor.

| Component | Header | Notes |
|---|---|---|
| `Transform` | `Components/TransformComponent.h` | Always present; created by `Entity` constructor. See [Transform Component](#transform-component) |
| `RenderComponent` | `Components/RenderComponent.h` | Mesh + material references; determines inclusion in `m_renderables` |
| `PhysicsComponent` | `Components/PhysicsComponent.h` | PhysX `PxRigidActor` + `PxShape`; see `07-Physics.md` |
| `AudioComponent` | `Components/AudioComponent.h` | FMOD sound channel references |
| `InputComponent` | `Components/InputComponent.h` | Player input state bindings |
| `AnimationComponent` | `Components/AnimationComponent.h` | Skeletal / keyframe animation data |
| `AIComponent` | `Components/AIComponent.h` | AI agent behaviour data |
| `ParticleEmitterComponent` | `Components/ParticleEmitterComponent.h` | Particle emitter settings fed to `ParticleSystem` |
| `CameraComponent` | `Components/CameraComponent.h` | Camera parameters; `AddComponent<CameraComponent>()` passes `m_transform` to its constructor |

Each entity slot accepts **at most one** instance of a given component type. Attempting to add a duplicate logs `FL_ENGINE_ERROR` and leaves the existing component untouched.

---

## Transform Component

**Header:** `Falcon/Core/Components/TransformComponent.h`

`Transform` inherits `BasicComponent` and is the only component guaranteed to exist on every entity. It stores the local transform relative to a parent matrix injected by `SceneNode` and lazily recomputes its world matrices via a dirty flag.

**Private state:**

| Field | Type | Description |
|---|---|---|
| `m_position` | `glm::vec3` | Local position in parent space |
| `m_rotation` | `glm::quat` | Local rotation quaternion |
| `m_scale` | `glm::vec3` | Local scale per axis |
| `m_model` | `glm::mat4` | World model matrix — recomputed when `m_updateFlag == true` |
| `m_normal` | `glm::mat3` | Normal matrix (`inverseTranspose(m_model)`) |
| `m_parentMatrix` | `glm::mat4` | Parent's world matrix, injected by `SceneNode::UpdateEntity()` |
| `m_front` | `glm::vec3` | Forward direction, default `{1, 0, 0}` |
| `m_updateFlag` | `bool` | Dirty flag; set `true` by any setter call |
| `octreeID` | `IDVector` | Stack of Octree node indices this transform currently occupies |

**Setters** (each sets `m_updateFlag = true`):

| Setter | Description |
|---|---|
| `SetPosition(vec3)` | Updates local position |
| `SetRotation(quat)` | Updates local rotation |
| `SetScale(vec3)` | Updates local scale |
| `SetRelativeSpace(mat4)` | Injects a new parent world matrix |

**Getters:**

| Getter | Return type | Description |
|---|---|---|
| `GetPosition()` | `const glm::vec3&` | Local position |
| `GetRotation()` | `const glm::quat&` | Local rotation quaternion |
| `GetScale()` | `const glm::vec3&` | Local scale |
| `GetModel()` | `const glm::mat4&` | World model matrix |
| `GetNormal()` | `const glm::mat3&` | World normal matrix |
| `GetFront()` | `const glm::vec3&` | Forward direction vector |
| `GetParent()` | `glm::mat4` | Stored parent world matrix |
| `GetFlag()` | `bool` | Current dirty flag value |

**Matrix recalculation** (`RecalculateMatrix()`, called internally when the dirty flag is set):

`m_model = scale(rotate(translate(m_parentMatrix, m_position), m_rotation), m_scale)`

`m_normal = inverseTranspose(m_model)`

**Octree helpers:**

| Method | Description |
|---|---|
| `pushOTID(value)` | Pushes an Octree node index onto the stack |
| `popOTID()` | Pops the top index |
| `topOTID()` | Returns the top index, or `-1` if empty |
| `GetOTID()` | Returns `const IDVector&` of all current Octree indices |
| `ClearOTID()` | Clears the entire stack |

---

## SceneNode Hierarchy

**Header:** `Falcon/Core/Scene/SceneGraph.h` — `namespace Scene`

`SceneNode` is the tree node wrapper around one `Entity*`. The full scene is a single tree rooted at `SceneGraph::m_rootNode`.

**Data members:**

| Member | Type | Description |
|---|---|---|
| `m_entity` | `Entity*` | Entity this node represents |
| `m_parent` | `SceneNode*` | Parent node; `nullptr` for the root |
| `m_childNodes` | `nodeVector` | Ordered list of child node pointers |
| `m_ReferenceMatrix` | `glm::mat4` | World matrix propagated down to children each frame |
| `m_status` | `Status` | `Active` or `Inactive` |
| `m_updateFlag` | `bool` | Whether this node needs to re-propagate its transform |

**Status enum:**

| Value | Meaning |
|---|---|
| `Status::Active` | Entity participates in update loops |
| `Status::Inactive` | Entity is skipped during scene update |

**Key methods:**

| Method | Description |
|---|---|
| `AddChild(node)` | Appends `node` to `m_childNodes` |
| `RemoveChild(node)` | Removes child by pointer |
| `RemoveChild(index)` | Removes child by index |
| `SetStatus(status)` | Sets `Active` or `Inactive` |
| `UpdateEntity(vector)` | Injects `m_ReferenceMatrix` into the entity's `Transform` via `SetRelativeSpace()` and pushes active entities into the supplied `entityVector` |
| `GetChildren()` | Returns `nodeVector&` |
| `GetChildByIndex(i)` | Returns `SceneNode*` at index `i` |
| `GetParent()` | Returns `SceneNode*` parent |
| `GetEntity()` | Returns `Entity*` |
| `GetWM()` | Returns `glm::mat4` world reference matrix |

`UpdateEntity()` is the mechanism by which parent transforms are pushed into child `Transform` components each frame — it is called recursively through the tree by `SceneGraph::UpdateScene()`.

---

## SceneGraph

**Header:** `Falcon/Core/Scene/SceneGraph.h` — `namespace Scene`

`SceneGraph` owns the full entity hierarchy for one loaded scene.

**Private members:**

| Member | Type | Description |
|---|---|---|
| `m_entityList` | `entityVector` | All entities in the scene |
| `m_renderables` | `entityVector` | Entities that have a `RenderComponent` |
| `m_updatedRenderables` | `entityVector` | Renderables whose transforms changed this frame (for Octree) |
| `m_rootNode` | `SceneNode*` | Root of the SceneNode tree |
| `m_terrain` | `Terrain*` | Loaded terrain object |
| `m_threads` | `ThreadPool*` | Shared `ThreadPool` singleton reference |

### Construction

`SceneGraph(const char* sceneFilePath)` parses a JSON file using rapidjson and builds the node tree in the following order:

| Phase | What happens |
|---|---|
| `"terrain"` | Calls `EntityManager::CreateTerrainEntity()` and adds the result to `m_entityList` |
| `"sky"` | Creates a sky entity with its `RenderComponent` set to the sky mesh |
| `"entities"` array | For each entry: calls `CreateNode()` → `EntityManager::CreateEntity()` → attaches all components specified in the template file |
| `"vehicles"` array | For each entry: creates a `PxRigidDynamic` actor, calls `physics::vehicle::CreateCar()`, pushes a `CarEvent` to `CarEventSystem` to register it with `CarSystem` |

After all entities are loaded, `SegregateEntities()` walks `m_entityList` and partitions entities that have a `RenderComponent` into `m_renderables`. The remaining entities (physics-only, audio-only, etc.) stay in `m_entityList`.

### Public API

| Method | Returns | Description |
|---|---|---|
| `GetRenderables()` | `entityVector&` | Entities consumed by `Renderer` each frame |
| `GetEntities()` | `entityVector*` | All entities; passed to `physics::StepPhysics` |
| `GetOctreeEntities()` | `entityVector&` | Transform-dirty renderables; consumed by `Octree::Update()` |
| `UpdateScene()` | `void` | Recursively walks the SceneNode tree, propagating parent matrices and rebuilding `m_updatedRenderables` |

`UpdateScene()` is called once per frame from `Engine::IngameUpdate()` before rendering. Entities whose `Transform::GetFlag()` is `true` after the walk are added to `m_updatedRenderables`.

---

## EntityManager

**Header:** `Falcon/Core/EntityManager.h`

All methods are `static` — `EntityManager` is a pure static utility class with no instance.

| Method | Parameters | Description |
|---|---|---|
| `CreateEntity` | `objTemplate, pos, rot, scale, actor` | Reads a JSON component template, constructs an `Entity` at the given transform, attaches components described in the template, and assigns the supplied `RigidbodyDynamic*` to its `PhysicsComponent` |
| `CreateTerrainEntity` | `terrainMesh` | Creates a static entity with a `RenderComponent` set to the terrain mesh |
| `SaveScene` | `sceneFilePath` | Serialises the current scene back to JSON |
| `CloseScene` | *(none)* | Releases all entity and scene resources; called during `Engine::~Engine()` |

`CreateEntity` is the primary factory called for every entry in the `"entities"` JSON array. The `objTemplate` path points to a separate JSON file that lists which components to add and their configuration values.

---

## JSON Scene Format

Scene files are loaded by `SceneGraph(const char* sceneFilePath)`.

**Top-level keys:**

| Key | Type | Description |
|---|---|---|
| `"terrain"` | object | Terrain mesh path, scale, and material |
| `"sky"` | object | Sky mesh reference |
| `"entities"` | array | Array of entity descriptor objects |
| `"vehicles"` | array | Array of vehicle descriptor objects |

**Entity descriptor fields** (each element of `"entities"`):

| Field | Type | Description |
|---|---|---|
| `"name"` | string | Entity identifier used for parent lookups |
| `"pos"` | `[x, y, z]` | World-space spawn position |
| `"rot"` | `[x, y, z, w]` | Rotation as a quaternion |
| `"scale"` | `[x, y, z]` | Scale per axis |
| `"parent"` | string | Name of the parent entity; empty string for root-level entities |
| `"template"` | string | Path to the JSON component template file |

**Vehicle descriptor fields** (each element of `"vehicles"`):

| Field | Type | Description |
|---|---|---|
| `"user_car"` | bool | `true` = player-controlled; `false` = AI-controlled |
| `"car_pos"` | `[x, y, z]` | Spawn position |
| `"car_scale"` | `[x, y, z]` | Scale |
| `"car_rot"` | `[x, y, z, w]` | Initial rotation quaternion |

---

## Scene Lifecycle

| Phase | Call | Notes |
|---|---|---|
| Load | `SceneGraph(sceneFilePath)` | Parses JSON, creates all entities, registers vehicles with `CarSystem` |
| Segregate | `SegregateEntities()` | Partitions `m_entityList` into `m_renderables` and the general list — called once at end of construction |
| Per-frame update | `UpdateScene()` | Propagates parent matrices, rebuilds `m_updatedRenderables` — called before rendering |
| Render query | `GetRenderables()` | `Renderer` calls this each frame to get its draw list |
| Octree query | `GetOctreeEntities()` | `Octree` calls this to update spatial partitioning for moved objects |
| Physics query | `GetEntities()` | `physics::StepPhysics` iterates all entities to sync `PxTransform` back to `Transform` |
| Save | `EntityManager::SaveScene(path)` | Serialises current entity state to disk |
| Shutdown | `EntityManager::CloseScene()` | Frees all scene resources; must run before `CarSystem::ShutDown()` and `physics::ShutdownPhysX()` |