#pragma once

#include "Nova/Core/Layer.h"
#include "Nova/Core/Log.h"
#include "Nova/Renderer/DemoTriangle.h"
#include "Nova/Renderer/Framebuffer.h"
#include "Nova/Renderer/RenderCommand.h"

#include <glm/glm.hpp>
#include <vector>

namespace Nova
{
    // The editor shell: dockspace, menu bar, Viewport, Console and Statistics panels.
    // Hierarchy / Inspector / Asset Browser arrive in Phases 3-4, when there is real data behind them.
    class EditorLayer final : public Layer
    {
    public:
        EditorLayer() : Layer("EditorLayer") {}

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float dt) override;
        void OnGuiRender() override;

    private:
        void DrawDockspace();
        void DrawMenuBar();
        void DrawViewport();
        void DrawConsole();
        void DrawStatistics();
        void DrawAbout();

        Scope<Framebuffer> m_Framebuffer;
        Scope<DemoTriangle> m_Triangle;
        RendererInfo m_RendererInfo;

        glm::vec2 m_ViewportSize{1280.0f, 720.0f}; // set by the Viewport panel, applied in OnUpdate
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        float m_Time = 0.0f;
        float m_FrameTime = 0.0f;

        bool m_ShowConsole = true;
        bool m_ShowStatistics = true;
        bool m_ShowAbout = false;

        // Console panel keeps a cached copy of the log and refreshes it only when the log changes.
        std::vector<LogEntry> m_ConsoleEntries;
        std::uint64_t m_ConsoleRevision = ~0ull;
        bool m_ConsoleScrollToBottom = false;
    };
} // namespace Nova
