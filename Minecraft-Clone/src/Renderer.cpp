#include "Renderer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
    :m_lineShader("res/shaders/line.shader")
{
    m_lineVBLayout.Push<float>(3);

    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    GLCall(glEnable(GL_DEPTH_TEST));
}

Renderer::~Renderer()
{}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib) const
{
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, float width, const glm::vec3& color, const glm::mat4& viewMatrix)
{
    float vertices[] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z,
    };

    m_linesVB.SetData(vertices, sizeof(vertices));
    m_linesVA.AddBuffer(m_linesVB, m_lineVBLayout);

    m_lineShader.Bind();
    m_lineShader.SetUniformMat4f("uViewMat", viewMatrix);
    m_lineShader.SetUniform3f("uColor", color.r, color.g, color.b);
    m_linesVA.Bind();
    GLCall(glLineWidth(width));
    GLCall(glDrawArrays(GL_LINES, 0, 2));
    GLCall(glLineWidth(1.0f));
    m_lineShader.Unbind();
}

void Renderer::DrawBoxOutline(const glm::vec3& start, const glm::vec3& end, float width, glm::vec3& color, const glm::mat4& viewMatrix)
{
    float vertices[] = {
        //bottom
        start.x, start.y, start.z,
        end.x, start.y, start.z,
        end.x, start.y, end.z,
        start.x, start.y, end.z,

        //top
        start.x, end.y, start.z,
        end.x, end.y, start.z,
        end.x, end.y, end.z,
        start.x, end.y, end.z
    };

    uint32_t indices[] = {
        //bottom
        0,1,
        1,2,
        2,3,
        3,0,

        // top
        4,5,
        5,6,
        6,7,
        7,4,

        // sides
        0,4,
        1,5,
        2,6,
        3,7
    };

    m_linesVB.SetData(vertices, sizeof(vertices));
    m_linesVA.AddBuffer(m_linesVB, m_lineVBLayout);

    m_linesIB.SetData(indices, sizeof(indices) / sizeof(uint32_t));

    m_lineShader.Bind();
    m_lineShader.SetUniformMat4f("uViewMat", viewMatrix);
    m_lineShader.SetUniform3f("uColor", color.r, color.g, color.b);
    GLCall(glLineWidth(width));
    m_linesVA.Bind();
    m_linesIB.Bind();
    GLCall(glDrawElements(GL_LINES, m_linesIB.GetCount(), GL_UNSIGNED_INT, nullptr));
    GLCall(glLineWidth(1.0f));
    m_lineShader.Unbind();
}

void Renderer::SetProjectionMatrix(const glm::mat4& projMatrix)
{
    m_projMatrix = projMatrix;
    m_lineShader.Bind();
    m_lineShader.SetUniformMat4f("uProjMat", projMatrix);
    m_lineShader.Unbind();
}

void Renderer::Clear() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}