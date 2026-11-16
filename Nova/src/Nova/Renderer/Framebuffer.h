#pragma once

#include <cstdint>

namespace Nova
{
    struct FramebufferSpec
    {
        std::uint32_t Width = 1280;
        std::uint32_t Height = 720;
    };

    // Off-screen render target (RGBA8 colour + depth/stencil), created with OpenGL 4.5 DSA calls.
    // The editor renders the scene into one and shows its colour texture inside the Viewport panel.
    class Framebuffer
    {
    public:
        explicit Framebuffer(const FramebufferSpec& spec);
        ~Framebuffer();

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        void Bind() const; // also sets the GL viewport to the framebuffer size
        static void Unbind();
        void Resize(std::uint32_t width, std::uint32_t height);

        std::uint32_t GetColorAttachment() const { return m_Color; }
        const FramebufferSpec& GetSpec() const { return m_Spec; }

    private:
        void Invalidate();
        void Release();

        FramebufferSpec m_Spec;
        std::uint32_t m_Fbo = 0;
        std::uint32_t m_Color = 0;
        std::uint32_t m_Depth = 0;
    };
} // namespace Nova
