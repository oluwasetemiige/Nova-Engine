# Nova Engine

A modern, lightweight, open-source 3D game engine and editor in C++20 / OpenGL 4.5.
Architecture and workflow are inspired by The Cherno's Hazel; no Hazel source code or assets are used.

> **Status: Phase 1 of 6.** The foundation is in place; most engine features are not built yet.
> See [Roadmap](#roadmap) for exactly what exists.

## What works in Phase 1

- GLFW window with an OpenGL 4.5 core context (GLAD loader)
- Application loop with a layer stack and delta time
- Dear ImGui (docking) editor shell: dockspace, menu bar, **Viewport**, **Console**, **Statistics**
- Viewport renders into an off-screen framebuffer (a rotating triangle smoke test) and resizes with the panel
- Console panel shows the engine log live (colour-coded, clearable)
- Dependency-free unit tests for the core (layers, main loop, logging)

## Requirements

| Tool | Version |
|------|---------|
| CMake | 3.20+ |
| C++20 compiler with `<format>` | MSVC 2022 (17.2+), GCC 13+, Clang 17+ |
| Python 3 + `jinja2` | any recent (GLAD generates its loader at configure time) |
| Git | any |
| GPU/driver | **OpenGL 4.5** support |

Linux also needs X11 development packages:
`sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev`

GLFW, GLM, Dear ImGui and GLAD are downloaded automatically by CMake (internet required on first configure).

## Build and run

**Windows**
```bat
git clone <your-repo-url> NovaEngine
cd NovaEngine
pip install jinja2
build.bat
build\NovaEditor\Release\NovaEditor.exe
```

**Linux**
```bash
pip install jinja2          # or: sudo apt install python3-jinja2
./build.sh
./build/NovaEditor/NovaEditor
```

**Plain CMake**
```bash
cmake -S . -B build
cmake --build build --config Release --parallel
```

### Core tests only (no downloads, no GPU)
```bash
cmake -S . -B build-tests -DNOVA_BUILD_EDITOR=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

## Project layout

```
Nova/          engine library
  src/Nova/Core        platform-independent core (Application, Layers, Log, Window interface)  [NovaCore]
  src/Nova/Platform    GLFW window
  src/Nova/Renderer    framebuffer, shader, render commands, Phase 1 demo triangle
  src/Nova/ImGui       Dear ImGui integration
NovaEditor/    the editor application
Tests/         NovaCore unit tests
cmake/         dependency fetching
docs/          architecture notes
```

## Known limitations

- **OpenGL 4.5 is required.** Many older Intel integrated GPUs only expose OpenGL 4.0-4.4 and will fail at startup
  with a clear error. Supporting them would mean lowering the baseline (e.g. to 4.1 / 3.3, no DSA calls) - a decision for a later phase.
- macOS is not supported (Apple's OpenGL stops at 4.1).
- Screenshots are not included yet; add them under `docs/images/` once the editor has been run on real hardware.

## Roadmap

| Phase | Scope | Status |
|-------|-------|--------|
| 1 | Architecture, window, OpenGL init, editor shell, basic viewport | Implemented; core tested, editor/renderer **not yet compiled or run** (see docs/ARCHITECTURE.md) |
| 2 | 3D rendering, camera, model import, textures, materials | Not started |
| 3 | ECS, hierarchy, inspector, entity manipulation | Not started |
| 4 | Asset browser, scene serialization, project management | Not started |
| 5 | Scripting, runtime, physics, Play/Pause/Stop | Not started |
| 6 | Packaging, docs, example game, release | Not started |

## License

MIT - see [LICENSE](LICENSE).
