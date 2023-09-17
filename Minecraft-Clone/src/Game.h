#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "FastNoiseLite/FastNoiseLite.h"
#include <unordered_set>
#include <queue>

constexpr int CHUNKS_RADIUS = 8;

class Game
{
private:
	Window m_Window;

	Camera m_Camera;

	Renderer m_Renderer;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	FastNoiseLite m_NoiseGen;

	Chunk m_chunks[CHUNKS_RADIUS * CHUNKS_RADIUS * 4];

	Shader m_shader;

	glm::mat4 m_projMatrix;

	std::unordered_set<std::string> m_loadedChunks;
	std::queue<uint32_t> m_freePlaces;


public:
	Game(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	~Game();

	void Run();

private:
	void InitializeScene();

	void Update();
	void Draw();
};

