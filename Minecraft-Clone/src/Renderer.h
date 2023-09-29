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
	VertexBuffer m_linesVB;
	IndexBuffer m_linesIB;
	VertexArray m_linesVA;
	Shader m_lineShader;

	Shader m_HUDShader;
	VertexBufferLayout m_textureVBLayout;
	VertexBuffer m_textureVB;
	IndexBuffer m_textureIB;
	VertexArray m_textureVA;

	glm::mat4 m_projMatrix;



public:
	Renderer();
	~Renderer();

	void Draw(const VertexArray& va, const IndexBuffer& ib) const;

	void DrawLine(const glm::vec3& start, const glm::vec3& end, float width, const glm::vec3& color, const glm::mat4& viewMatrix);
	void DrawBoxOutline(const glm::vec3& start, const glm::vec3& end, float width, glm::vec3& color, const glm::mat4& viewMatrix);

	void Draw2DTexture(const glm::vec2& corner, float width, float height, int textureSlot, float wndWidth, float wndHeight);

	void SetProjectionMatrix(const glm::mat4& projMatrix);

	void Clear() const;
};

