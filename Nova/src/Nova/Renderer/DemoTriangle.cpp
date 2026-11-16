#include "Nova/Renderer/DemoTriangle.h"

#include <glad/gl.h>

namespace Nova
{
    namespace
    {
        const char* kVertexSource = R"(#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
uniform float uTime;
uniform float uAspect;
out vec3 vColor;
void main()
{
    float c = cos(uTime), s = sin(uTime);
    vec2 p = vec2(c * aPos.x - s * aPos.y, s * aPos.x + c * aPos.y);
    gl_Position = vec4(p.x / uAspect, p.y, 0.0, 1.0);
    vColor = aColor;
}
)";

        const char* kFragmentSource = R"(#version 450 core
in vec3 vColor;
out vec4 oColor;
void main() { oColor = vec4(vColor, 1.0); }
)";

        // x, y, r, g, b
        constexpr float kVertices[] = {
            -0.6f, -0.5f, 1.0f, 0.2f, 0.2f,
             0.6f, -0.5f, 0.2f, 1.0f, 0.2f,
             0.0f,  0.6f, 0.2f, 0.4f, 1.0f,
        };
    } // namespace

    DemoTriangle::DemoTriangle() : m_Shader(CreateScope<Shader>(kVertexSource, kFragmentSource))
    {
        glCreateBuffers(1, &m_Vbo);
        glNamedBufferStorage(m_Vbo, sizeof(kVertices), kVertices, 0);

        glCreateVertexArrays(1, &m_Vao);
        glVertexArrayVertexBuffer(m_Vao, 0, m_Vbo, 0, 5 * sizeof(float));

        glEnableVertexArrayAttrib(m_Vao, 0);
        glVertexArrayAttribFormat(m_Vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_Vao, 0, 0);

        glEnableVertexArrayAttrib(m_Vao, 1);
        glVertexArrayAttribFormat(m_Vao, 1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
        glVertexArrayAttribBinding(m_Vao, 1, 0);
    }

    DemoTriangle::~DemoTriangle()
    {
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);
    }

    void DemoTriangle::Draw(float timeSeconds, float aspectRatio) const
    {
        m_Shader->Bind();
        m_Shader->SetFloat("uTime", timeSeconds);
        m_Shader->SetFloat("uAspect", aspectRatio);
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
} // namespace Nova
