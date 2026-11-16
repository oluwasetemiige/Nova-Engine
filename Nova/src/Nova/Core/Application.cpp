#include "Nova/Core/Application.h"

#include "Nova/Core/Assert.h"

#include <algorithm>

namespace Nova
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpec spec, Scope<Window> window)
        : m_Spec(std::move(spec)), m_Window(std::move(window))
    {
        NOVA_ASSERT(s_Instance == nullptr, "Only one Application may exist at a time");
        NOVA_ASSERT(m_Window != nullptr, "Application requires a window");
        s_Instance = this;
    }

    Application::~Application()
    {
        s_Instance = nullptr;
    }

    Application& Application::Get()
    {
        NOVA_ASSERT(s_Instance != nullptr, "No Application exists");
        return *s_Instance;
    }

    void Application::Run()
    {
        Log::Info("Running '{}'", m_Spec.Name);

        double lastTime = m_Window->GetTime();

        while (m_Running && !m_Window->ShouldClose())
        {
            const double now = m_Window->GetTime();
            // Clamp so a debugger pause or window drag does not produce a giant timestep.
            m_LastFrameTime = static_cast<float>(std::clamp(now - lastTime, 0.0, 0.25));
            lastTime = now;

            for (auto& layer : m_Layers)
                layer->OnUpdate(m_LastFrameTime);

            if (m_GuiHost)
            {
                m_GuiHost->BeginFrame();
                for (auto& layer : m_Layers)
                    layer->OnGuiRender();
                m_GuiHost->EndFrame();
            }

            m_Window->OnUpdate();
            ++m_FrameCount;
        }

        Log::Info("'{}' stopped after {} frames", m_Spec.Name, m_FrameCount);
    }
} // namespace Nova
