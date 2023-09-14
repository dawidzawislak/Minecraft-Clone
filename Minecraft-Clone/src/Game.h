#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "FastNoiseLite/FastNoiseLite.h"

class Game
{
private:
	Window m_Window;

	Camera m_Camera;

	Renderer m_Renderer;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	FastNoiseLite m_NoiseGen;

	BlockTextureManager m_BlockTextureManager;

	Chunk m_testChunk;
	int noIndices;

	VertexBuffer m_vb;
	VertexArray m_va;
	IndexBuffer m_ib;
	Shader m_shader;

	glm::mat4 m_projMatrix;

public:
	Game(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	~Game();

	void Run();

private:
	void InitializeScene();

	void Update();
	void Draw();
};

