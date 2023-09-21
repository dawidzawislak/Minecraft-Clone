#include "Game.h"

#include "Input.h"
#include <iostream>

#include "BlocksDB.h"

#include "ChunkLoader.h"

#include "Physics.h"

static const glm::vec3 PLAYER_INITIAL_POS = glm::vec3(0.0f, 200.0f, 0.0f);
static const glm::vec3 PLAYER_INITIAL_POS2 = glm::vec3(0.0f, 150.0f, 0.0f);

Game::Game(std::string title, unsigned int width, unsigned int height, bool fullScreen)
	: m_Window(title, width, height, fullScreen), m_shader("res/shaders/default.shader"), m_Player(PLAYER_INITIAL_POS)//, m_Camera(PLAYER_INITIAL_POS2)
{
	BlockTextureManager::Initialize("res/textures/block");
	BlocksDB::Initialize();

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
	m_projMatrix = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 2000.0f);
	m_Renderer.SetProjectionMatrix(m_projMatrix);

	ChunkLoader::Initialize(CHUNKS_RADIUS, PLAYER_INITIAL_POS, SEED);

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

	/// --- Updating systems --------------------------------------------------------------------------
	m_Player.Update(m_deltaTime);
	Physics::Update(m_deltaTime);
	// FPS camera
	
	Transform* playerTransform = EntityRegistry::GetComponent<Transform>(m_Player.GetPlayerID());
	Transform* cameraTransform = EntityRegistry::GetComponent<Transform>(m_Camera.GetCameraID());
	cameraTransform->position = playerTransform->position;
	cameraTransform->front = playerTransform->front;
	cameraTransform->up = playerTransform->up;
	
	/*
	if (Input::IsKeyPressed(GLFW_KEY_UP))
		m_Camera.ProcessKeyboard(Direction::FORWARD, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		m_Camera.ProcessKeyboard(Direction::BACKWARD, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		m_Camera.ProcessKeyboard(Direction::LEFT, m_deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		m_Camera.ProcessKeyboard(Direction::RIGHT, m_deltaTime);

	m_Camera.ProcessMouseMovement(Input::GetMouseXOffset(), Input::GetMouseYOffset());
	*/
	ChunkLoader::Update(cameraTransform->position);
	//ChunkLoader::Update(m_Camera.m_cameraPos);
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
}