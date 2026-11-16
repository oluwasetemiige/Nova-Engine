#pragma once

#include "Nova/Core/Base.h"
#include "Nova/Core/Layer.h"

#include <cstddef>
#include <vector>

namespace Nova
{
    // Owns layers. Regular layers sit below overlays; within each group, push order is bottom-to-top.
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        LayerStack(const LayerStack&) = delete;
        LayerStack& operator=(const LayerStack&) = delete;

        // Both call OnAttach() and return a non-owning pointer.
        Layer* PushLayer(Scope<Layer> layer);
        Layer* PushOverlay(Scope<Layer> overlay);

        std::size_t Size() const { return m_Layers.size(); }

        auto begin() { return m_Layers.begin(); }
        auto end() { return m_Layers.end(); }

    private:
        std::vector<Scope<Layer>> m_Layers;
        std::size_t m_InsertIndex = 0; // boundary between layers and overlays
    };
} // namespace Nova
