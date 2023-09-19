#pragma once

#include <GL/glew.h>
#include <vector>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "BlockTextureManager.h"
#include "Chunk.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
	VertexBufferLayout m_lineVBLayout;
	VertexBuffer m_lineVB;
	VertexArray m_lineVA;

	Shader m_lineShader;

	glm::mat4 m_projMatrix;

public:
	Renderer();
	~Renderer();

	void Draw(const VertexArray& va, const IndexBuffer& ib) const;

	void DrawLine(const glm::vec3& start, const glm::vec3& end, float width, const glm::vec3& color, const glm::mat4& viewMatrix);

	void SetProjectionMatrix(const glm::mat4& projMatrix);

	void Clear() const;
};

