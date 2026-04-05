# Getting Started

This guide covers cloning the repository, downloading dependencies, generating project files, and building the engine on Windows and Linux.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Clone the Repository](#clone-the-repository)
- [Download Dependencies](#download-dependencies)
- [Generate Project Files](#generate-project-files)
- [Open and Configure in Visual Studio](#open-and-configure-in-visual-studio)
- [Build — Windows](#build--windows)
- [Build — Linux](#build--linux)
- [Run](#run)
- [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Windows

| Requirement | Notes |
|---|---|
| Visual Studio 2026 | With **Desktop development with C++** workload |
| MSVC v180 toolset | Included with VS 2026 |
| Windows 10 / 11 x64 | 64-bit only |
| Git | For cloning |

### Linux

```
sudo apt-get update
sudo apt install python3 python3-dev
sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev
```

---

## Clone the Repository

```
git clone https://github.com/Sadanand14/FALCON_ENGINE.git
cd FALCON_ENGINE
```

---

## Download Dependencies

Two archives must be downloaded and extracted before the project will build.

### 1 — Vendor Libraries

Contains all third-party headers and prebuilt libs (GLAD, Boost, GLFW, GLM, Assimp, ozz, PhysX, FMOD Studio, spdlog, Nuklear).

| Platform | Download | Extract to |
|---|---|---|
| Windows | [vendors.zip](https://drive.google.com/open?id=1CchZegqYkadK3kZguiG7o85_ZGr8fIU9) | `FALCON_ENGINE\Falcon\Falcon\` |
| Linux | [Vendor.tar.zip](https://drive.google.com/open?id=18iscICo1bDoGShJAOcVhI2Q6F5Ayr7OM) | `FALCON_ENGINE/Falcon/Falcon/` |

### 2 — Game Assets

Contains models, scenes, shaders, sounds, and textures.

> **Note:** The Windows assets link requires a `@g.rit.edu` Google account.

| Platform | Download | Extract to |
|---|---|---|
| Windows | [Windows Assets](https://drive.google.com/a/g.rit.edu/file/d/1lXzUblckjqBw5ZdnKEeIOGwsa4X3WoRd/view?usp=sharing) | `FALCON_ENGINE\Falcon\` |
| Linux | [Linux tarball](https://drive.google.com/open?id=1V2WRjAMlHCOTJoYXXKZzNuE7PgAqw7UY) | `FALCON_ENGINE/Falcon/build/Debug/` |

After extraction the vendor folder should look like this:

```
FALCON_ENGINE\Falcon\Falcon\
├── boost\
├── fmod\
├── glad\
├── glfw\
├── glm\
├── physx\
├── premake5.exe
└── ...
```

---

## Generate Project Files

Falcon uses **Premake5** as its build configuration tool. `premake5.exe` is included inside the extracted vendor archive.

**Windows:**

```
.\GenProject.bat
```

**Linux:**

```
./GenProject.sh
```

This generates `FALCON_ENGINE.sln`, `Falcon\Falcon.vcxproj`, and `FALCON_GAME\FALCON_GAME.vcxproj` from `premake5.lua`.

> **Important:** Any time you add a new source directory, library, or include path, update `premake5.lua` before regenerating. Failing to do so will cause linker errors and missing includes that are difficult to diagnose.

---

## Open and Configure in Visual Studio

1. Open `FALCON_ENGINE.sln` in Visual Studio 2026.
2. If prompted with **"Retarget Projects"**, accept — this upgrades both projects from `v142` to `v180`.
3. To retarget manually: right-click the solution → **Retarget Solution** → select **v180**.
4. Verify both projects show **v180** under __Project Properties > General > Platform Toolset__.

---

## Build — Windows

1. Set the configuration to **Debug | x64**.
2. Press **Ctrl+Shift+B** or go to __Build > Build Solution__.
3. `Falcon.lib` builds first; `FALCON_GAME.exe` links against it automatically.

---

## Build — Linux

```
make
```

To clean the build directory:

```
make clean
```

---

## Run

Set `FALCON_GAME` as the startup project in Visual Studio and press **F5**.

The executable resolves assets via the relative path `../Assets/`. Visual Studio sets the working directory correctly by default. If running the binary outside the IDE, launch it from within the `FALCON_GAME\` output folder.

---

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| `MSB8020` — toolset `v142` not found | Projects still targeting VS2019 toolset | Retarget solution to `v180` |
| `C1083` — cannot open `glad/glad.h` | `vendors.zip` not extracted to the correct path | Verify contents land in `Falcon\Falcon\` |
| Assets fail to load at runtime | Wrong extraction path or working directory | Extract to `Falcon\`; run from IDE |
| No audio at startup | See known issues in engine | Refer to [Audio](06-Audio.md) |
| Build errors after adding a new file | `premake5.lua` not updated | Add the new path to `premake5.lua` and re-run `GenProject.bat` |