#include "Nova/Core/LayerStack.h"

#include "Nova/Core/Assert.h"

namespace Nova
{
    LayerStack::~LayerStack()
    {
        // Detach top-down so upper layers can still rely on lower ones while shutting down.
        while (!m_Layers.empty())
        {
            m_Layers.back()->OnDetach();
            m_Layers.pop_back();
        }
    }

    Layer* LayerStack::PushLayer(Scope<Layer> layer)
    {
        NOVA_ASSERT(layer != nullptr, "PushLayer received a null layer");
        Layer* raw = layer.get();
        m_Layers.insert(m_Layers.begin() + static_cast<std::ptrdiff_t>(m_InsertIndex), std::move(layer));
        ++m_InsertIndex;
        raw->OnAttach();
        return raw;
    }

    Layer* LayerStack::PushOverlay(Scope<Layer> overlay)
    {
        NOVA_ASSERT(overlay != nullptr, "PushOverlay received a null layer");
        Layer* raw = overlay.get();
        m_Layers.push_back(std::move(overlay));
        raw->OnAttach();
        return raw;
    }
} // namespace Nova
