#include "EditorLayer.h"

#include "Nova/Core/Application.h"

#include <imgui.h>
#include <imgui_internal.h> // DockBuilder: used only to create the default panel layout on first run

#include <cstdint>

namespace Nova
{
    void EditorLayer::OnAttach()
    {
        m_Framebuffer = CreateScope<Framebuffer>(FramebufferSpec{1280, 720});
        m_Triangle = CreateScope<DemoTriangle>();
        m_RendererInfo = RenderCommand::GetInfo();
        Log::Info("Nova Editor ready");
    }

    void EditorLayer::OnDetach()
    {
        // Free GPU resources while the OpenGL context still exists.
        m_Triangle.reset();
        m_Framebuffer.reset();
    }

    void EditorLayer::OnUpdate(float dt)
    {
        m_FrameTime = dt;
        m_Time += dt;

        // Apply the size requested by the Viewport panel last frame.
        const auto w = static_cast<std::uint32_t>(m_ViewportSize.x);
        const auto h = static_cast<std::uint32_t>(m_ViewportSize.y);
        m_Framebuffer->Resize(w, h);

        const auto& spec = m_Framebuffer->GetSpec();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.08f, 0.09f, 0.11f, 1.0f});
        RenderCommand::Clear();
        m_Triangle->Draw(m_Time, static_cast<float>(spec.Width) / static_cast<float>(spec.Height));
        Framebuffer::Unbind();
    }

    void EditorLayer::OnGuiRender()
    {
        DrawDockspace();
        DrawViewport();
        if (m_ShowConsole) DrawConsole();
        if (m_ShowStatistics) DrawStatistics();
        if (m_ShowAbout) DrawAbout();
    }

    void EditorLayer::DrawDockspace()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        const ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("NovaDockHost", nullptr, flags);
        ImGui::PopStyleVar(3);

        const ImGuiID dockspaceId = ImGui::GetID("NovaDockSpace");

        // First run (no saved layout): viewport in the middle, console below, statistics on the right.
        if (ImGui::DockBuilderGetNode(dockspaceId) == nullptr)
        {
            ImGui::DockBuilderRemoveNode(dockspaceId);
            ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->WorkSize);

            ImGuiID center = dockspaceId;
            const ImGuiID bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.25f, nullptr, &center);
            const ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.22f, nullptr, &center);

            ImGui::DockBuilderDockWindow("Viewport", center);
            ImGui::DockBuilderDockWindow("Console", bottom);
            ImGui::DockBuilderDockWindow("Statistics", right);
            ImGui::DockBuilderFinish(dockspaceId);
        }

        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        DrawMenuBar();
        ImGui::End();
    }

    void EditorLayer::DrawMenuBar()
    {
        if (!ImGui::BeginMenuBar())
            return;

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit", "Alt+F4"))
                Application::Get().Close();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Console", nullptr, &m_ShowConsole);
            ImGui::MenuItem("Statistics", nullptr, &m_ShowStatistics);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About Nova Engine", nullptr, &m_ShowAbout);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    void EditorLayer::DrawViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar();

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        const ImVec2 avail = ImGui::GetContentRegionAvail();
        if (avail.x >= 1.0f && avail.y >= 1.0f)
        {
            m_ViewportSize = {avail.x, avail.y};
            // OpenGL's origin is bottom-left, ImGui's is top-left: flip V.
            ImGui::Image((ImTextureID)(intptr_t)m_Framebuffer->GetColorAttachment(), avail, ImVec2(0, 1),
                         ImVec2(1, 0));
        }
        ImGui::End();
    }

    void EditorLayer::DrawConsole()
    {
        const std::uint64_t revision = Log::Revision();
        if (revision != m_ConsoleRevision)
        {
            m_ConsoleEntries = Log::Entries();
            m_ConsoleRevision = revision;
            m_ConsoleScrollToBottom = true;
        }

        if (!ImGui::Begin("Console", &m_ShowConsole))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Clear"))
            Log::Clear();
        ImGui::SameLine();
        ImGui::TextDisabled("%zu messages", m_ConsoleEntries.size());
        ImGui::Separator();

        ImGui::BeginChild("ConsoleScroll", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        for (const LogEntry& entry : m_ConsoleEntries)
        {
            ImVec4 color(0.85f, 0.85f, 0.85f, 1.0f);
            const char* tag = "INFO ";
            switch (entry.Level)
            {
            case LogLevel::Trace: color = ImVec4(0.55f, 0.55f, 0.55f, 1.0f); tag = "TRACE"; break;
            case LogLevel::Info: break;
            case LogLevel::Warn: color = ImVec4(1.0f, 0.82f, 0.30f, 1.0f); tag = "WARN "; break;
            case LogLevel::Error: color = ImVec4(1.0f, 0.38f, 0.38f, 1.0f); tag = "ERROR"; break;
            }
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::Text("[%s] %s", tag, entry.Message.c_str());
            ImGui::PopStyleColor();
        }
        if (m_ConsoleScrollToBottom)
        {
            ImGui::SetScrollHereY(1.0f);
            m_ConsoleScrollToBottom = false;
        }
        ImGui::EndChild();
        ImGui::End();
    }

    void EditorLayer::DrawStatistics()
    {
        if (ImGui::Begin("Statistics", &m_ShowStatistics))
        {
            ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
            ImGui::Text("Frame time: %.2f ms", static_cast<double>(m_FrameTime) * 1000.0);
            ImGui::Separator();
            ImGui::Text("Viewport: %.0f x %.0f", static_cast<double>(m_ViewportSize.x),
                        static_cast<double>(m_ViewportSize.y));
            ImGui::Text("Viewport focused: %s", m_ViewportFocused ? "yes" : "no");
            ImGui::Text("Viewport hovered: %s", m_ViewportHovered ? "yes" : "no");
            ImGui::Separator();
            ImGui::TextWrapped("GPU: %s", m_RendererInfo.Renderer.c_str());
            ImGui::TextWrapped("Vendor: %s", m_RendererInfo.Vendor.c_str());
            ImGui::TextWrapped("OpenGL: %s", m_RendererInfo.Version.c_str());
        }
        ImGui::End();
    }

    void EditorLayer::DrawAbout()
    {
        if (ImGui::Begin("About Nova Engine", &m_ShowAbout, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Nova Engine 0.1.0 (Phase 1)");
            ImGui::Text("An open-source C++20 / OpenGL 4.5 game engine.");
            ImGui::Separator();
            ImGui::Text("Built with GLFW, GLAD, GLM and Dear ImGui.");
        }
        ImGui::End();
    }
} // namespace Nova
