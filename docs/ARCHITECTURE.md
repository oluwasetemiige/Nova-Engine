# Architecture (Phase 1)

```
            +---------------- NovaEditor (exe) ----------------+
            |  main.cpp -> Application + ImGuiHost + EditorLayer |
            +------------------------+---------------------------+
                                     |
                +--------------------v--------------------+
                |  Nova (lib)  - needs GPU + 3rd parties  |
                |  Platform/GLFW  Renderer/*  ImGui/*     |
                +--------------------+--------------------+
                                     |
                +--------------------v--------------------+
                |  NovaCore (lib) - no dependencies       |
                |  Application  LayerStack  Log           |
                |  interfaces: Window, GuiHost, Layer     |
                +-----------------------------------------+
```

## Why the split
`NovaCore` only knows the abstract `Window` and `GuiHost` interfaces. The GLFW/OpenGL/ImGui implementations live in
`Nova`. This keeps the main loop and layer logic unit-testable on any machine (the tests use a fake window) and keeps
the editor/runtime separation open for Phase 5, where a game runtime will reuse `Nova` without ImGui.

## Frame flow
1. `Application::Run` computes delta time (clamped to 0.25 s).
2. Every layer's `OnUpdate(dt)`, bottom to top. `EditorLayer` renders the scene into its `Framebuffer`.
3. `GuiHost::BeginFrame`, every layer's `OnGuiRender`, `GuiHost::EndFrame` (clears the window, draws ImGui).
4. `Window::OnUpdate` swaps buffers and polls events.

## Lifetime rules
`Application` destroys layers first (they own GPU objects), then the GUI host, then the window (which owns the GL context).
`EditorLayer::OnDetach` releases its GPU resources explicitly.

## Verification status
| Part | Status |
|------|--------|
| NovaCore + Tests | Compiled with GCC 13 (`-Wall -Wextra -Wpedantic -Werror`), run under ASan/UBSan: all pass |
| Nova (GLFW, renderer, ImGui) and NovaEditor | **Written but not compiled or run.** Authored in a sandbox without network access, CMake or GLFW/ImGui/GLAD. Dependency versions in `cmake/Dependencies.cmake` (GLFW 3.4, GLM 1.0.1, ImGui v1.91.9-docking, GLAD v2.0.8) are unverified here; expect to fix small compile/CMake errors on first build. |
