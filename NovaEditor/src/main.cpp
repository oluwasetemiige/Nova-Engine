#include "EditorLayer.h"

#include "Nova/Core/Application.h"
#include "Nova/Core/Log.h"
#include "Nova/ImGui/ImGuiHost.h"

#include <cstdlib>
#include <exception>

int main()
{
    try
    {
        Nova::WindowProps window;
        window.Title = "Nova Editor";
        window.Width = 1600;
        window.Height = 900;

        Nova::ApplicationSpec spec;
        spec.Name = "Nova Editor";

        Nova::Application app(spec, Nova::Window::Create(window));
        app.SetGuiHost(Nova::CreateScope<Nova::ImGuiHost>(app.GetWindow()));
        app.PushLayer(Nova::CreateScope<Nova::EditorLayer>());
        app.Run();
    }
    catch (const std::exception& e)
    {
        Nova::Log::Error("Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
