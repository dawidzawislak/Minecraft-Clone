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
	float m_lastX;
	float m_lastY;
	bool m_firstMouse = true;

	Renderer m_Renderer;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	FastNoiseLite m_NoiseGen;

public:
	Game(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	~Game();

	void Run();

private:
	void InitializeScene();

	void Update();
	void Draw();
};

