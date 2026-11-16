# NovaCore: platform-independent engine core. No third-party dependencies,
# so it builds (and is unit-tested) anywhere with a C++20 compiler.
add_library(NovaCore STATIC
    src/Nova/Core/Application.cpp
    src/Nova/Core/LayerStack.cpp
    src/Nova/Core/Log.cpp)
target_include_directories(NovaCore PUBLIC src)
target_compile_features(NovaCore PUBLIC cxx_std_20)
nova_set_warnings(NovaCore)

if(NOVA_BUILD_EDITOR)
    include(${PROJECT_SOURCE_DIR}/cmake/Dependencies.cmake)

    # Nova: windowing, OpenGL renderer and ImGui integration on top of NovaCore.
    add_library(Nova STATIC
        src/Nova/Platform/GLFW/GlfwWindow.cpp
        src/Nova/Renderer/DemoTriangle.cpp
        src/Nova/Renderer/Framebuffer.cpp
        src/Nova/Renderer/RenderCommand.cpp
        src/Nova/Renderer/Shader.cpp
        src/Nova/ImGui/ImGuiHost.cpp)
    target_link_libraries(Nova
        PUBLIC  NovaCore glm::glm imgui
        PRIVATE glfw glad_gl)
    target_compile_definitions(Nova PRIVATE GLFW_INCLUDE_NONE) # GLAD supplies the GL headers
    nova_set_warnings(Nova)
endif()
