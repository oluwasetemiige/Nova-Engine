#include "Nova/Platform/GLFW/GlfwWindow.h"

#include "Nova/Core/Log.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Nova
{
    namespace
    {
        int s_WindowCount = 0; // GLFW is initialised for the first window, terminated after the last

        void GlfwErrorCallback(int error, const char* description)
        {
            Log::Error("GLFW error {}: {}", error, description);
        }
    } // namespace

    Scope<Window> Window::Create(const WindowProps& props)
    {
        return CreateScope<GlfwWindow>(props);
    }

    GlfwWindow::GlfwWindow(const WindowProps& props) : m_Width(props.Width), m_Height(props.Height)
    {
        if (s_WindowCount == 0)
        {
            glfwSetErrorCallback(GlfwErrorCallback);
            if (!glfwInit())
                throw std::runtime_error("Failed to initialise GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height),
                                    props.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            if (s_WindowCount == 0)
                glfwTerminate();
            throw std::runtime_error(
                "Could not create an OpenGL 4.5 window. Update your graphics driver; "
                "GPUs/drivers without OpenGL 4.5 support (some older Intel iGPUs) cannot run Nova.");
        }
        ++s_WindowCount;

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
        {
            Destroy();
            throw std::runtime_error("Failed to load OpenGL functions (GLAD)");
        }

        SetVSync(props.VSync);
        Log::Info("Window created: {}x{}", m_Width, m_Height);
        Log::Info("OpenGL {} | {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)),
                  reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    }

    GlfwWindow::~GlfwWindow()
    {
        Destroy();
    }

    void GlfwWindow::Destroy()
    {
        if (!m_Window)
            return;
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        if (--s_WindowCount == 0)
            glfwTerminate();
    }

    void GlfwWindow::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        int w = 0, h = 0;
        glfwGetWindowSize(m_Window, &w, &h);
        m_Width = static_cast<std::uint32_t>(w);
        m_Height = static_cast<std::uint32_t>(h);
    }

    bool GlfwWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    void GlfwWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    double GlfwWindow::GetTime() const
    {
        return glfwGetTime();
    }
} // namespace Nova
