#pragma once

namespace Nova
{
    // Brackets the per-frame GUI pass: BeginFrame() -> every Layer::OnGuiRender() -> EndFrame().
    // The ImGui implementation lives in Nova/ImGui; tests use a counting fake.
    class GuiHost
    {
    public:
        virtual ~GuiHost() = default;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
    };
} // namespace Nova
