#include "Game.h"

#include "Input.h"
#include <iostream>

#include "BlocksDB.h"

#include "ChunkLoader.h"

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
	ChunkLoader::ReleaseChunks();

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
	m_Camera.SetCameraPosition(glm::vec3(0.0f, 150.0f, 0.0f));
	m_projMatrix = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 2000.0f);
	m_Renderer.SetProjectionMatrix(m_projMatrix);

	ChunkLoader::Initialize(CHUNKS_RADIUS, m_Camera.GetCameraPosition(), SEED);

	BlockTextureManager::BindTextureAtlasAndUVTextureBuffer();
	m_shader.Bind();
	m_shader.SetUniform1i("uTexture", 0);
	m_shader.SetUniform1i("uUVsTexture", 1);
	m_shader.SetUniform3f("uSunPosition", 100.0f, 300.0f, 100.0f);
	m_shader.SetUniformMat4f("uProjMat", m_projMatrix);
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

	ChunkLoader::Update(m_Camera.GetCameraPosition());
}
void Game::Draw()
{
	m_Renderer.Clear();

	glm::mat4 viewMat = m_Camera.GetViewMatrix();

	// Rendering Chunks
	m_shader.Bind();
	m_shader.SetUniformMat4f("uViewMat", viewMat);
	
	for (Chunk* chunk : ChunkLoader::GetLoadedChunks()) {
		glm::ivec2 chunkPos = chunk->GetPosition();
		m_shader.SetUniform2i("uChunkPosition", chunkPos.x, chunkPos.y);
		
		m_Renderer.Draw(chunk->renderData.va, chunk->renderData.ib);
	}
	m_shader.Unbind();

	// Rendering lines
	m_Renderer.DrawLine(glm::vec3(0.0f, 150.0f, -1.0f), glm::vec3(5.0f, 150.0f, -0.5f), 5, glm::vec3(1.0f, 0.0f, 0.0f), viewMat);
	m_Renderer.DrawLine(glm::vec3(5.0f, 150.0f, -0.5f), glm::vec3(5.0f, 150.0f, 1.5f), 5, glm::vec3(1.0f, 1.0f, 0.0f), viewMat);
}