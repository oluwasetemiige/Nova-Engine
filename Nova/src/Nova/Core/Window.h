#pragma once

#include "Nova/Core/Base.h"

#include <cstdint>
#include <string>

namespace Nova
{
    struct WindowProps
    {
        std::string Title = "Nova Engine";
        std::uint32_t Width = 1600;
        std::uint32_t Height = 900;
        bool VSync = true;
    };

    // Platform-independent window interface. The only implementation today is GLFW
    // (Platform/GLFW); the interface keeps NovaCore free of platform code and testable.
    class Window
    {
    public:
        virtual ~Window() = default;

        // Call once per frame: presents the frame and processes OS events.
        virtual void OnUpdate() = 0;

        virtual std::uint32_t GetWidth() const = 0;
        virtual std::uint32_t GetHeight() const = 0;
        virtual bool ShouldClose() const = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual double GetTime() const = 0; // seconds since window system start

        // Backend handle (GLFWwindow* for the GLFW backend), used by the ImGui integration.
        virtual void* GetNativeWindow() const = 0;

        // Defined by the platform layer (Nova library). Throws std::runtime_error on failure.
        static Scope<Window> Create(const WindowProps& props = {});
    };
} // namespace Nova
