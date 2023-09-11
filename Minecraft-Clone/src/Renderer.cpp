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
    :m_blockShader("res/shaders/basic.shader")
{
    float vertices[] = {
        // front
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // lt 0
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // lb 1
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // rb 2
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // rt 3

         // back
        -1.0f,  1.0f,  -1.0f, 1.0f, 1.0f, // lt 4
        -1.0f, -1.0f,  -1.0f, 1.0f, 0.0f, // lb 5 
         1.0f, -1.0f,  -1.0f, 0.0f, 0.0f, // rb 6
         1.0f,  1.0f,  -1.0f, 0.0f, 1.0f, // rt 7

         // up fix
         -1.0f,  1.0f,  -1.0f, 0.0f, 0.0f, // back lt' 8
          1.0f,  1.0f,  -1.0f, 1.0f, 0.0f, // back rt' 9

          // down fix
          -1.0f, -1.0f,  -1.0f, 0.0f, 1.0f, // back lb' 10 
           1.0f, -1.0f,  -1.0f, 1.0f, 1.0f, // back rb' 11
    };

    unsigned int indicesFront[] = {
        // front
        0,1,3,
        1,2,3
    };

    unsigned int indicesBack[] = {
        // back
        7,5,4,
        7,6,5
    };
    unsigned int indicesRight[] = {
        // right
        3,2,7,
        2,6,7
    };
    unsigned int indicesLeft[] = {
        //left
        4,5,0,
        5,1,0
    };
    unsigned int indicesDown[] = {
        //down
        1,10,2,
        10,11,2
    };
    unsigned int indicesUp[] = {
        //up
        8,0,9,
        0,3,9
    };

    m_cubeVB.SetData(vertices, sizeof(vertices));
    m_cubeLayout.Push<float>(3);
    m_cubeLayout.Push<float>(2);
    m_cubeVA.AddBuffer(m_cubeVB, m_cubeLayout);

    m_cubeBackIB.SetData(indicesBack, 6);
    m_cubeFrontIB.SetData(indicesFront, 6);
    m_cubeUpIB.SetData(indicesUp, 6);
    m_cubeBottomIB.SetData(indicesDown, 6);
    m_cubeLeftIB.SetData(indicesLeft, 6);
    m_cubeRightIB.SetData(indicesRight, 6);

    m_blockTextureManager.Initialize("res/textures/tiles.png");
}

Renderer::~Renderer()
{
    m_blockTextureManager.Release();
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawBlocks(const std::vector<Block>& blocks, const glm::mat4& vp)
{
    m_blockShader.Bind();
    m_cubeVA.Bind();

    for (const Block& block : blocks) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, block.GetPosVec3() * 0.4f);
        model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
        
        m_blockShader.SetUniform1i("u_Texture", 0);
        glm::mat4 mvp = vp * model;
        m_blockShader.SetUniformMat4f("u_MVP", mvp);

        m_blockTextureManager.BindTopTexture(block.GetType());
        Draw(m_cubeVA, m_cubeUpIB, m_blockShader);

        m_blockTextureManager.BindSideTexture(block.GetType());
        Draw(m_cubeVA, m_cubeFrontIB, m_blockShader);
        Draw(m_cubeVA, m_cubeRightIB, m_blockShader);
        Draw(m_cubeVA, m_cubeBackIB, m_blockShader);
        Draw(m_cubeVA, m_cubeLeftIB, m_blockShader);

        m_blockTextureManager.BindBottomTexture(block.GetType());
        Draw(m_cubeVA, m_cubeBottomIB, m_blockShader);
    }
}

void Renderer::Clear() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}