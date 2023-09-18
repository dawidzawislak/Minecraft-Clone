#include "Game.h"

#include "Input.h"
#include <iostream>

#include "BlocksDB.h"

template<typename R>
bool is_ready(std::future<R> const& f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

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
	/// TODO: cleanup every chunk

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

static std::queue<Chunk*> m_freedChunks;

static std::mutex m;

static Chunk* CreateChunk(int posX, int posZ)
{
	m.lock();
	if (m_freedChunks.size() > 0) {
		Chunk* chunk = m_freedChunks.front();
		m_freedChunks.pop();
		m.unlock();
		chunk->LoadCPU(posX, posZ, SEED);

		return chunk;
	}
	m.unlock();
	Chunk* chunk = new Chunk();
	chunk->LoadCPU(posX, posZ, SEED);
	return chunk;
}

void Game::InitializeScene()
{
	m_Camera.SetCameraPosition(glm::vec3(0.0f, 150.0f, 0.0f));
	m_projMatrix = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 2000.0f);

	glm::vec3 playerPos = m_Camera.GetCameraPosition();

	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);

	for (int i = -CHUNKS_RADIUS + playerPosChunkCoords.x; i < CHUNKS_RADIUS + playerPosChunkCoords.x; i++) {
		for (int j = -CHUNKS_RADIUS + playerPosChunkCoords.y; j < CHUNKS_RADIUS + playerPosChunkCoords.y; j++) {
			if (i*i + j*j <= CHUNKS_RADIUS_SQUARED) {
				m_chunksToCreateQueue.push(std::async(std::launch::async, CreateChunk, i, j));
				m_loadedChunks.insert(std::to_string(i) + ":" + std::to_string(j));
			}
		}
	}

	BlockTextureManager::BindTextureAtlasAndUVTextureBuffer();
	m_shader.Bind();
	m_shader.SetUniform1i("uTexture", 0);
	m_shader.SetUniform1i("uUVsTexture", 1);
	m_shader.SetUniform3f("uSunPosition", 100.0f, 300.0f, 100.0f);
}

void Game::SynchronizeThreads()
{
	while (m_chunksToCreateQueue.size() > 0 && is_ready(m_chunksToCreateQueue.front())) {
		m_chunks.push_back(m_chunksToCreateQueue.front().get());
		m_chunksToCreateQueue.pop();
		m_chunks.back()->LoadGPU();
	}
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
	for (int i = 0; i < m_chunks.size(); i++) {

		glm::ivec2 chunkPos = m_chunks[i]->GetPosition();

		if (pow(chunkPos.x - playerPosChunkCoords.x, 2) + pow(chunkPos.y - playerPosChunkCoords.y, 2) > CHUNKS_RADIUS * CHUNKS_RADIUS) {
			Chunk* chunk = m_chunks[i];
			chunk->ReleaseGPU();
			//m_chunksToDeleteQueue.push(std::async(std::launch::async, DeleteChunk, m_chunks[i]));
			m_loadedChunks.erase(std::to_string(chunk->GetPosition().x) + ":" + std::to_string(chunk->GetPosition().y));
			m_chunks.erase(m_chunks.begin() + i);
			m_freedChunks.push(chunk);
		}
	}
	
	// Add new chunk if needed
	for (int i = -CHUNKS_RADIUS + playerPosChunkCoords.x; i < CHUNKS_RADIUS + playerPosChunkCoords.x; i++) {
		for (int j = -CHUNKS_RADIUS + playerPosChunkCoords.y; j < CHUNKS_RADIUS + playerPosChunkCoords.y; j++) {
			if (pow(i - playerPosChunkCoords.x, 2) + pow(j - playerPosChunkCoords.y, 2) <= pow(CHUNKS_RADIUS, 2) && m_loadedChunks.count(std::to_string(i) + ":" + std::to_string(j)) == 0) {
				m_chunksToCreateQueue.push(std::async(std::launch::async, CreateChunk, i, j));
				m_loadedChunks.insert(std::to_string(i) + ":" + std::to_string(j));
			}
		}
	}

	SynchronizeThreads();
}
void Game::Draw()
{
	glm::mat4 viewMat = m_Camera.GetViewMatrix();
	m_shader.SetUniformMat4f("uProjMat", m_projMatrix);
	m_shader.SetUniformMat4f("uViewMat", viewMat);

	glm::vec3 playerPos = m_Camera.GetCameraPosition();

	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);

	m_Renderer.Clear();

	for (Chunk* chunk : m_chunks) {
		if (!chunk->loaded) continue;

		glm::ivec2 chunkPos = chunk->GetPosition();
		m_shader.SetUniform2i("uChunkPosition", chunkPos.x, chunkPos.y);
		
		m_Renderer.Draw(chunk->renderData.va, chunk->renderData.ib);
	}
}