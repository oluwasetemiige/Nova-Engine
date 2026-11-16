#pragma once

#include <cstdint>

namespace Nova
{
    // Minimal GLSL program: vertex + fragment stage compiled from source strings.
    // Throws std::runtime_error (with the driver's log) if compilation or linking fails.
    class Shader
    {
    public:
        Shader(const char* vertexSource, const char* fragmentSource);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void Bind() const;
        void SetFloat(const char* name, float value) const;

    private:
        std::uint32_t m_Program = 0;
    };
} // namespace Nova
