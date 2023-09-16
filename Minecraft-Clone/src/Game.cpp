#include "Game.h"

#include "Input.h"
#include <iostream>

#include "BlocksDB.h"

Game::Game(std::string title, unsigned int width, unsigned int height, bool fullScreen)
	: m_Window(title, width, height, fullScreen), m_shader("res/shaders/default.shader")
{
	BlockTextureManager::Initialize("res/textures/block");
	BlocksDB::Initialize();
	Input::SetCameraBinding(&m_Camera);

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
	m_Camera.SetCameraPosition(glm::vec3(0.0f, 200.0f, 2.0f));
	m_projMatrix = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 200.0f);

	VertexBufferLayout layout;
	layout.Push<uint32_t>(1);
	layout.Push<uint32_t>(1);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_chunks[j  * 8 + i].Generate(i-4, j-4, 1234);

			m_vb[j * 8 + i].SetData(m_chunks[j * 8 + i].renderData.vertices.data(), m_chunks[j * 8 + i].renderData.vertices.size() * sizeof(UVVertex));
			m_va[j * 8 + i].AddBuffer(m_vb[j * 8 + i], layout);
			m_ib[j * 8 + i].SetData(m_chunks[j * 8 + i].renderData.indices.data(), m_chunks[j * 8 + i].renderData.indices.size());
		}
	}

	BlockTextureManager::BindTextureAtlasAndUVTextureBuffer();
	m_shader.Bind();
	m_shader.SetUniform1i("uTexture", 0);
	m_shader.SetUniform1i("uUVsTexture", 1);
	m_shader.SetUniform3f("uSunPosition", 100.0f, 300.0f, 100.0f);
}

void Game::Update()
{
	// Update frame times
	float currentFrame = static_cast<float>(glfwGetTime());
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	m_Window.SetWindowTitle("Minecraft Clone   | FPS: " + std::to_string((int)(1 / m_deltaTime)));

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
	glm::mat4 viewMat = m_Camera.GetViewMatrix();
	m_shader.SetUniformMat4f("uProjMat", m_projMatrix);
	m_shader.SetUniformMat4f("uViewMat", viewMat);

	m_Renderer.Clear();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			glm::ivec2 chunkPos = m_chunks[j * 8 + i].GetPosition();
			m_shader.SetUniform2i("uChunkPosition", chunkPos.x, chunkPos.y);
			m_va[j * 8 + i].Bind();
			m_ib[j * 8 + i].Bind();
			GLCall(glDrawElements(GL_TRIANGLES, m_chunks[j * 8 + i].renderData.indices.size(), GL_UNSIGNED_INT, 0));
		}
	}
}