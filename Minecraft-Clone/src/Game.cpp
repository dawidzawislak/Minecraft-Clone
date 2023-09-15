#include "Game.h"

#include "Input.h"
#include <iostream>

#include "BlocksDB.h"

Game::Game(std::string title, unsigned int width, unsigned int height, bool fullScreen)
	: m_Window(title, width, height, fullScreen), m_shader("res/shaders/basic.shader")
{
	BlockTextureManager::Initialize("res/textures/block");
	BlocksDB::Initialize();
	InitializeScene();
}

Game::~Game()
{
	BlockTextureManager::Release();
}

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
	m_Camera.SetCameraPosition(glm::vec3(0.0f, 128.0f, 4*16.0f + 2.0f));
	m_projMatrix = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 200.0f);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_chunks[j  * 8 + i].Generate(i-4, j-4, 123);

			m_vb[j * 8 + i].SetData(m_chunks[j * 8 + i].renderData.vertices.data(), m_chunks[j * 8 + i].renderData.vertices.size() * sizeof(UVVertex));
			VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(2);
			m_va[j * 8 + i].AddBuffer(m_vb[j * 8 + i], layout);

			m_ib[j * 8 + i].SetData(m_chunks[j * 8 + i].renderData.indices.data(), m_chunks[j * 8 + i].renderData.indices.size());

		}
	}
	BlockTextureManager::BindTextureAtlas(0);
	m_shader.Bind();
	m_shader.SetUniform1i("u_Texture", 0);

	GLCall(glEnable(GL_DEPTH_TEST));
	Input::SetCameraBinding(&m_Camera);
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

	// Calculations
}
void Game::Draw()
{
	glm::mat4 mvp = m_projMatrix * m_Camera.GetViewMatrix();
	m_shader.SetUniformMat4f("u_MVP", mvp);
	m_Renderer.Clear();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_va[j * 8 + i].Bind();
			m_ib[j * 8 + i].Bind();
			GLCall(glDrawElements(GL_TRIANGLES, m_chunks[j * 8 + i].renderData.indices.size(), GL_UNSIGNED_INT, 0));
		}
	}
}