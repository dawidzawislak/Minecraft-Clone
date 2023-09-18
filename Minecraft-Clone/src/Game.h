#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "FastNoiseLite/FastNoiseLite.h"
#include <unordered_set>
#include <queue>
#include <future>

constexpr int CHUNKS_RADIUS = 16;
constexpr int CHUNKS_RADIUS_SQUARED = CHUNKS_RADIUS * CHUNKS_RADIUS;
static constexpr int SEED = 123;

class Game
{
private:
	Window m_Window;

	Camera m_Camera;

	Renderer m_Renderer;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	FastNoiseLite m_NoiseGen;

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

	void SynchronizeThreads();
};

