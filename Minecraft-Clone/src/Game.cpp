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
	for (int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS * 4; i++)
		m_freePlaces.push(i);

	m_Camera.SetCameraPosition(glm::vec3(0.0f, 150.0f, 0.0f));
	m_projMatrix = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 2000.0f);

	glm::vec3 playerPos = m_Camera.GetCameraPosition();

	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);

	for (int i = -CHUNKS_RADIUS + playerPosChunkCoords.x; i < CHUNKS_RADIUS + playerPosChunkCoords.x; i++) {
		for (int j = -CHUNKS_RADIUS + playerPosChunkCoords.y; j < CHUNKS_RADIUS + playerPosChunkCoords.y; j++) {
			if (i*i + j*j <= CHUNKS_RADIUS * CHUNKS_RADIUS) {
				uint32_t index = m_freePlaces.front();
				m_freePlaces.pop();
				m_chunks[index].SetChunkData(i, j, 123);
				m_chunks[index].CreateRenderData();
				m_loadedChunks.insert(std::to_string(i) + ":" + std::to_string(j));
				m_chunks[index].loaded = true;
			}
		}
	}

	int t = m_loadedChunks.count("0:0");

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

	glm::vec3 playerPos = m_Camera.GetCameraPosition();

	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);
	
	// Remove chunk if needed
	for (int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS * 4; i++) {
		if (!m_chunks[i].loaded) continue;

		glm::ivec2 chunkPos = m_chunks[i].GetPosition();

		if (pow(chunkPos.x - playerPosChunkCoords.x, 2) + pow(chunkPos.y - playerPosChunkCoords.y, 2) > CHUNKS_RADIUS * CHUNKS_RADIUS) {
			m_loadedChunks.erase(std::to_string(chunkPos.x) + ":" + std::to_string(chunkPos.y));
			m_chunks[i].Release();
			m_freePlaces.push(i);
		}
	}
	// Add new chunk if needed
	for (int i = -CHUNKS_RADIUS + playerPosChunkCoords.x; i < CHUNKS_RADIUS + playerPosChunkCoords.x; i++) {
		for (int j = -CHUNKS_RADIUS + playerPosChunkCoords.y; j < CHUNKS_RADIUS + playerPosChunkCoords.y; j++) {
			if (pow(i - playerPosChunkCoords.x, 2) + pow(j - playerPosChunkCoords.y, 2) <= pow(CHUNKS_RADIUS, 2) && m_loadedChunks.count(std::to_string(i) + ":" + std::to_string(j)) == 0) {
				uint32_t index = m_freePlaces.front();
				m_freePlaces.pop();
				m_chunks[index].SetChunkData(i, j, 123);
				m_chunks[index].CreateRenderData();
				m_loadedChunks.insert(std::to_string(i) + ":" + std::to_string(j));
				m_chunks[index].loaded = true;
			}
		}
	}

}
void Game::Draw()
{
	glm::mat4 viewMat = m_Camera.GetViewMatrix();
	m_shader.SetUniformMat4f("uProjMat", m_projMatrix);
	m_shader.SetUniformMat4f("uViewMat", viewMat);

	glm::vec3 playerPos = m_Camera.GetCameraPosition();

	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);

	m_Renderer.Clear();

	for (int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS * 4; i++) {
		if (!m_chunks[i].loaded) continue;

		glm::ivec2 chunkPos = m_chunks[i].GetPosition();
		m_shader.SetUniform2i("uChunkPosition", chunkPos.x, chunkPos.y);
		m_Renderer.Draw(m_chunks[i].renderData.va, m_chunks[i].renderData.ib);
	}
}