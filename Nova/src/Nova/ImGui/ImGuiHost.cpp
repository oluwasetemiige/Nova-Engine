#include "Nova/ImGui/ImGuiHost.h"

#include "Nova/Renderer/RenderCommand.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Nova
{
    ImGuiHost::ImGuiHost(Window& window) : m_Window(window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
        io.IniFilename = "NovaEditor.ini"; // remembers the panel layout between runs

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 3.0f;
        style.FrameRounding = 3.0f;
        style.TabRounding = 3.0f;
        style.WindowBorderSize = 1.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.12f, 0.14f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.10f, 0.12f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.15f, 0.19f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.30f, 0.45f, 0.55f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.14f, 0.18f, 1.0f);
        style.Colors[ImGuiCol_TabSelected] = ImVec4(0.20f, 0.30f, 0.45f, 1.0f);

        auto* glfwWindow = static_cast<GLFWwindow*>(m_Window.GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    ImGuiHost::~ImGuiHost()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiHost::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiHost::EndFrame()
    {
        ImGui::Render();

        // Clear the real window; the scene itself was rendered earlier into the viewport framebuffer.
        auto* glfwWindow = static_cast<GLFWwindow*>(m_Window.GetNativeWindow());
        int w = 0, h = 0;
        glfwGetFramebufferSize(glfwWindow, &w, &h);
        RenderCommand::SetViewport(0, 0, static_cast<std::uint32_t>(w), static_cast<std::uint32_t>(h));
        RenderCommand::SetClearColor({0.06f, 0.06f, 0.07f, 1.0f});
        RenderCommand::Clear();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
} // namespace Nova
