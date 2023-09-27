#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "FastNoiseLite/FastNoiseLite.h"
#include <unordered_set>
#include <queue>
#include <future>
#include "Player.h"

constexpr int CHUNKS_RADIUS = 24;
constexpr int CHUNKS_RADIUS_SQUARED = CHUNKS_RADIUS * CHUNKS_RADIUS;
static constexpr int SEED = 123;

class Game
{
private:
	Window m_Window;

	Player m_Player;
	Camera m_Camera;

	Renderer m_Renderer;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	FastNoiseLite m_NoiseGen;

	Shader m_shader;

	glm::mat4 m_projMatrix;

	glm::ivec3 m_outline;
	glm::ivec3 m_blockToSet;
	bool m_blockDestroyed;
	bool m_blockSet;

public:
	Game(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	~Game();

	void Run();

	static std::vector<glm::vec3> boxes;

private:
	void InitializeScene();

	void Update();
	void Draw();

	void SynchronizeThreads();
};

