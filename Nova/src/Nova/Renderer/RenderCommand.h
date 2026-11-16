#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>

namespace Nova
{
    struct RendererInfo
    {
        std::string Vendor;
        std::string Renderer;
        std::string Version;
    };

    // Thin wrappers over raw OpenGL state calls, so the rest of the engine stays GL-free.
    class RenderCommand
    {
    public:
        static void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();
        static RendererInfo GetInfo();
    };
} // namespace Nova
