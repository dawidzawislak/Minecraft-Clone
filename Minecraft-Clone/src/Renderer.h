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
	//BlockTextureManager m_blockTextureManager;
	VertexArray m_cubeVA;
	IndexBuffer m_cubeFrontIB;
	IndexBuffer m_cubeBackIB;
	IndexBuffer m_cubeLeftIB;
	IndexBuffer m_cubeRightIB;
	IndexBuffer m_cubeBottomIB;
	IndexBuffer m_cubeUpIB;

	VertexBuffer m_cubeVB;
	VertexBufferLayout m_cubeLayout;

	Shader m_blockShader;

public:
	Renderer();
	~Renderer();

	void Draw(const VertexArray& va, const IndexBuffer& ib) const;

	void Clear() const;
};

