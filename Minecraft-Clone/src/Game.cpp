#include "Game.h"

#include "Input.h"

Game::Game(std::string title, unsigned int width, unsigned int height, bool fullScreen)
	: m_Window(title, width, height, fullScreen)
{}

Game::~Game()
{}

void Game::Run()
{
	while (!glfwWindowShouldClose(*m_Window.GetGLFWWindow())) {

		Update();
		Draw();

		glfwSwapBuffers(*m_Window.GetGLFWWindow());
		glfwPollEvents();
	}
}


void Game::InitializeScene()
{
	m_Camera.SetCameraPosition(glm::vec3(0.0f, 0.0f, 2.0f));
}

void Game::Update()
{
	// Update frame times
	float currentFrame = static_cast<float>(glfwGetTime());
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	// Input precessing
	if (Input::IsKeyPressed(GLFW_KEY_W))
		m_Camera.ProcessKeyboard(Direction::FORWARD, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_S))
		m_Camera.ProcessKeyboard(Direction::BACKWARD, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_A))
		m_Camera.ProcessKeyboard(Direction::LEFT, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_D))
		m_Camera.ProcessKeyboard(Direction::RIGHT, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_SPACE))
		m_Camera.ProcessKeyboard(Direction::UP, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		m_Camera.ProcessKeyboard(Direction::DOWN, m_deltaTime);

	m_Camera.ProcessMouseMovement(Input::GetMousePosXOffset(), Input::GetMousePosYOffset());

	// Calculations
}
void Game::Draw()
{
	m_Renderer.Clear();

	// Draw Stuff
}