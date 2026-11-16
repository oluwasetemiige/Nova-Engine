#pragma once

#include "Nova/Core/GuiHost.h"
#include "Nova/Core/Window.h"

namespace Nova
{
    // Dear ImGui (docking branch) integration for the GLFW + OpenGL 3 backends.
    class ImGuiHost final : public GuiHost
    {
    public:
        explicit ImGuiHost(Window& window);
        ~ImGuiHost() override;

        ImGuiHost(const ImGuiHost&) = delete;
        ImGuiHost& operator=(const ImGuiHost&) = delete;

        void BeginFrame() override;
        void EndFrame() override;

    private:
        Window& m_Window;
    };
} // namespace Nova
