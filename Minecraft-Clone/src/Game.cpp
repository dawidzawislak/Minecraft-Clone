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
	, m_crosshairTexture("res/textures/crosshair.png")
{
	BlockTextureManager::Initialize("res/textures/block");
	BlocksDB::Initialize();
	m_outline = glm::ivec3(0,0,0);
	InitializeScene();
	m_blockDestroyed = false;
	m_blockSet = false;
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
	if (m_deltaTime > 0.5f)
		m_deltaTime = 0.5f;

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

	// Ray casting
	glm::vec3 start = cameraTransform->position + glm::vec3(0.0f, 0.5f, 0.0f);
	glm::vec3 end = cameraTransform->position + glm::vec3(0.0f, 0.5f, 0.0f) + (cameraTransform->front * 5.0f);

	float len = glm::length(end - start);

	glm::vec3 pos = start;
	float lenOff = glm::length(cameraTransform->front * 0.1f);

	for (float i = 0.0f; i < len;) {
		pos += cameraTransform->front * 0.1f;

		if (ChunkLoader::GetBlock(glm::ivec3(floor(pos.x), floor(pos.y), floor(pos.z))) == BlockType::AIR) {
			i += lenOff;
			m_outline = glm::ivec3();
			m_blockToSet = glm::ivec3(floor(pos.x), floor(pos.y), floor(pos.z));
			continue;
		}

		m_outline = glm::ivec3(floor(pos.x), floor(pos.y), floor(pos.z));
		break;
	}

	if (Input::IsLeftMouseBtnDown() && m_outline != glm::ivec3() && !m_blockDestroyed) {
		ChunkLoader::SetBlock(m_outline, BlockType::AIR);
		m_outline = glm::ivec3();
		m_blockDestroyed = true;
	}
	else if (!Input::IsLeftMouseBtnDown())
		m_blockDestroyed = false;

	if (Input::IsRightMouseBtnDown() && !m_blockSet) {
		ChunkLoader::SetBlock(m_blockToSet, BlockType::STONE);
		m_blockSet = true;
	}
	else if (!Input::IsRightMouseBtnDown())
		m_blockSet = false;
	
	ChunkLoader::Update(cameraTransform->position);
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

	Transform* cameraTransform = EntityRegistry::GetComponent<Transform>(m_Camera.GetCameraID());

	glm::vec3 outlineColor(0.3f, 0.3f, 0.3f);

	m_Renderer.DrawBoxOutline(glm::vec3(m_outline) - glm::vec3(0.001f), glm::vec3(m_outline + glm::ivec3(1.0f)) + glm::vec3(0.001f), 3.0f, outlineColor, viewMat);
	
	m_crosshairTexture.Bind(3);
	m_Renderer.Draw2DTexture(glm::vec2((m_Window.GetWidth() / 2.0f) - 15.0f, (m_Window.GetHeight() / 2.0f) + 15.0f), 30.0f, 30.0f, 3, m_Window.GetWidth(), m_Window.GetHeight());
}