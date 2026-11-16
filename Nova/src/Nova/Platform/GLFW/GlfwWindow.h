#pragma once

#include "Nova/Core/Window.h"

struct GLFWwindow;

namespace Nova
{
    // GLFW + OpenGL 4.5 core window. Also loads the OpenGL function pointers (GLAD).
    class GlfwWindow final : public Window
    {
    public:
        explicit GlfwWindow(const WindowProps& props);
        ~GlfwWindow() override;

        void OnUpdate() override;

        std::uint32_t GetWidth() const override { return m_Width; }
        std::uint32_t GetHeight() const override { return m_Height; }
        bool ShouldClose() const override;
        void SetVSync(bool enabled) override;
        double GetTime() const override;
        void* GetNativeWindow() const override { return m_Window; }

    private:
        void Destroy();

        GLFWwindow* m_Window = nullptr;
        std::uint32_t m_Width = 0;
        std::uint32_t m_Height = 0;
    };
} // namespace Nova
