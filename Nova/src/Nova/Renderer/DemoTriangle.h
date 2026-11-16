#pragma once

#include "Nova/Core/Base.h"
#include "Nova/Renderer/Shader.h"

#include <cstdint>

namespace Nova
{
    // Phase 1 smoke-test geometry: a rotating coloured triangle that proves the whole
    // pipeline (context -> shader -> VAO -> framebuffer -> ImGui viewport) works.
    // It is replaced by the real mesh renderer in Phase 2.
    class DemoTriangle
    {
    public:
        DemoTriangle();
        ~DemoTriangle();

        DemoTriangle(const DemoTriangle&) = delete;
        DemoTriangle& operator=(const DemoTriangle&) = delete;

        void Draw(float timeSeconds, float aspectRatio) const;

    private:
        Scope<Shader> m_Shader;
        std::uint32_t m_Vao = 0;
        std::uint32_t m_Vbo = 0;
    };
} // namespace Nova
