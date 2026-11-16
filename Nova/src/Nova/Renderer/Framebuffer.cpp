#include "Nova/Renderer/Framebuffer.h"

#include "Nova/Core/Assert.h"

#include <glad/gl.h>

namespace Nova
{
    namespace
    {
        constexpr std::uint32_t kMaxSize = 8192;
    }

    Framebuffer::Framebuffer(const FramebufferSpec& spec) : m_Spec(spec)
    {
        Invalidate();
    }

    Framebuffer::~Framebuffer()
    {
        Release();
    }

    void Framebuffer::Release()
    {
        if (m_Fbo)
        {
            glDeleteFramebuffers(1, &m_Fbo);
            glDeleteTextures(1, &m_Color);
            glDeleteTextures(1, &m_Depth);
            m_Fbo = m_Color = m_Depth = 0;
        }
    }

    void Framebuffer::Invalidate()
    {
        Release();

        glCreateFramebuffers(1, &m_Fbo);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_Color);
        glTextureStorage2D(m_Color, 1, GL_RGBA8, static_cast<GLsizei>(m_Spec.Width),
                           static_cast<GLsizei>(m_Spec.Height));
        glTextureParameteri(m_Color, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_Color, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_Color, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_Color, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_Depth);
        glTextureStorage2D(m_Depth, 1, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Spec.Width),
                           static_cast<GLsizei>(m_Spec.Height));

        glNamedFramebufferTexture(m_Fbo, GL_COLOR_ATTACHMENT0, m_Color, 0);
        glNamedFramebufferTexture(m_Fbo, GL_DEPTH_STENCIL_ATTACHMENT, m_Depth, 0);

        NOVA_ASSERT(glCheckNamedFramebufferStatus(m_Fbo, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                    "Framebuffer is incomplete");
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
        glViewport(0, 0, static_cast<GLsizei>(m_Spec.Width), static_cast<GLsizei>(m_Spec.Height));
    }

    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Resize(std::uint32_t width, std::uint32_t height)
    {
        if (width == 0 || height == 0 || width > kMaxSize || height > kMaxSize)
            return; // ignore invalid sizes (e.g. a collapsed panel)
        if (width == m_Spec.Width && height == m_Spec.Height)
            return;
        m_Spec.Width = width;
        m_Spec.Height = height;
        Invalidate();
    }
} // namespace Nova
