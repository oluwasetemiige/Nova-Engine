#pragma once

#include <string>
#include <utility>

namespace Nova
{
    // A Layer is one self-contained slice of the application (editor UI, game, debug overlay).
    // Layers are updated bottom-to-top every frame.
    class Layer
    {
    public:
        explicit Layer(std::string name = "Layer") : m_Name(std::move(name)) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float /*deltaSeconds*/) {}
        virtual void OnGuiRender() {}

        const std::string& GetName() const { return m_Name; }

    private:
        std::string m_Name;
    };
} // namespace Nova
