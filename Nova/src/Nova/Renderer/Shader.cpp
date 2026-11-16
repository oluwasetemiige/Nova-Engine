#include "Nova/Renderer/Shader.h"

#include <glad/gl.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace Nova
{
    namespace
    {
        GLuint Compile(GLenum type, const char* source)
        {
            const GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            GLint ok = GL_FALSE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
            if (!ok)
            {
                GLint len = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
                std::vector<char> log(static_cast<std::size_t>(len > 1 ? len : 1));
                glGetShaderInfoLog(shader, len, nullptr, log.data());
                glDeleteShader(shader);
                throw std::runtime_error(std::string("Shader compile error: ") + log.data());
            }
            return shader;
        }
    } // namespace

    Shader::Shader(const char* vertexSource, const char* fragmentSource)
    {
        const GLuint vs = Compile(GL_VERTEX_SHADER, vertexSource);
        GLuint fs = 0;
        try
        {
            fs = Compile(GL_FRAGMENT_SHADER, fragmentSource);
        }
        catch (...)
        {
            glDeleteShader(vs);
            throw;
        }

        m_Program = glCreateProgram();
        glAttachShader(m_Program, vs);
        glAttachShader(m_Program, fs);
        glLinkProgram(m_Program);

        GLint ok = GL_FALSE;
        glGetProgramiv(m_Program, GL_LINK_STATUS, &ok);

        glDetachShader(m_Program, vs);
        glDetachShader(m_Program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        if (!ok)
        {
            GLint len = 0;
            glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &len);
            std::vector<char> log(static_cast<std::size_t>(len > 1 ? len : 1));
            glGetProgramInfoLog(m_Program, len, nullptr, log.data());
            glDeleteProgram(m_Program);
            m_Program = 0;
            throw std::runtime_error(std::string("Shader link error: ") + log.data());
        }
    }

    Shader::~Shader()
    {
        if (m_Program)
            glDeleteProgram(m_Program);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_Program);
    }

    void Shader::SetFloat(const char* name, float value) const
    {
        glProgramUniform1f(m_Program, glGetUniformLocation(m_Program, name), value);
    }
} // namespace Nova
