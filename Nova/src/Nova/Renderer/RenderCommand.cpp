#include "Nova/Renderer/RenderCommand.h"

#include <glad/gl.h>

namespace Nova
{
    void RenderCommand::SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
    {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width),
                   static_cast<GLsizei>(height));
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    RendererInfo RenderCommand::GetInfo()
    {
        auto str = [](GLenum name) {
            const auto* s = glGetString(name);
            return std::string(s ? reinterpret_cast<const char*>(s) : "unknown");
        };
        return {str(GL_VENDOR), str(GL_RENDERER), str(GL_VERSION)};
    }
} // namespace Nova
