#pragma once

#include "Nova/Core/Base.h"
#include "Nova/Core/GuiHost.h"
#include "Nova/Core/LayerStack.h"
#include "Nova/Core/Window.h"

#include <cstdint>
#include <string>

namespace Nova
{
    struct ApplicationSpec
    {
        std::string Name = "Nova Application";
    };

    // Owns the window, the GUI host and the layer stack, and runs the main loop.
    class Application
    {
    public:
        Application(ApplicationSpec spec, Scope<Window> window);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        // Runs until Close() is called or the window is closed.
        void Run();
        void Close() { m_Running = false; }

        Layer* PushLayer(Scope<Layer> layer) { return m_Layers.PushLayer(std::move(layer)); }
        Layer* PushOverlay(Scope<Layer> overlay) { return m_Layers.PushOverlay(std::move(overlay)); }
        void SetGuiHost(Scope<GuiHost> host) { m_GuiHost = std::move(host); }

        Window& GetWindow() { return *m_Window; }
        float GetLastFrameTime() const { return m_LastFrameTime; }
        std::uint64_t GetFrameCount() const { return m_FrameCount; }

        static Application& Get();

    private:
        ApplicationSpec m_Spec;

        // Destruction runs bottom-to-top: layers (which own GPU resources) die first,
        // then the GUI host, and only then the window that owns the graphics context.
        Scope<Window> m_Window;
        Scope<GuiHost> m_GuiHost;
        LayerStack m_Layers;

        bool m_Running = true;
        float m_LastFrameTime = 0.0f;
        std::uint64_t m_FrameCount = 0;

        static Application* s_Instance;
    };
} // namespace Nova
