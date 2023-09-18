#include "ChunkLoader.h"

#include <string>

std::vector<Chunk*> ChunkLoader::m_chunks;
int ChunkLoader::m_chunkRadius;
int ChunkLoader::m_seed;
std::unordered_set<std::string> ChunkLoader::m_loadedChunks;
std::queue<std::future<Chunk*>> ChunkLoader::m_chunksToCreateQueue;


static std::vector<Chunk*> freedChunks;
static std::mutex freedChunksMutex;

template<typename R>
bool is_ready(std::future<R> const& f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

static Chunk* CreateChunk(int posX, int posZ, int seed)
{
	Chunk* chunk = nullptr;
	freedChunksMutex.lock();
	if (freedChunks.size() > 0) {
		chunk = freedChunks.back();
		freedChunks.pop_back();
	}
	freedChunksMutex.unlock();

	if (chunk == nullptr)
		chunk = new Chunk();
	
	chunk->LoadCPU(posX, posZ, seed);

	return chunk;
}

void ChunkLoader::Initialize(uint32_t chunkRadius, glm::vec3 playerPos, int seed)
{
	m_chunkRadius = chunkRadius;
	m_seed = seed;

	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);

	for (int i = -m_chunkRadius + playerPosChunkCoords.x; i < m_chunkRadius + playerPosChunkCoords.x; i++) {
		for (int j = -m_chunkRadius + playerPosChunkCoords.y; j < m_chunkRadius + playerPosChunkCoords.y; j++) {
			if (i * i + j * j <= m_chunkRadius * m_chunkRadius) {
				m_chunksToCreateQueue.push(std::async(std::launch::async, CreateChunk, i, j, m_seed));
				m_loadedChunks.insert(std::to_string(i) + ":" + std::to_string(j));
			}
		}
	}
}

void ChunkLoader::Update(glm::vec3 playerPos)
{
	glm::ivec2 playerPosChunkCoords = glm::ivec2(playerPos.x / 16, playerPos.z / 16);

	// Remove chunk if needed
	for (int i = 0; i < m_chunks.size(); i++) {

		glm::ivec2 chunkPos = m_chunks[i]->GetPosition();

		if (pow(chunkPos.x - playerPosChunkCoords.x, 2) + pow(chunkPos.y - playerPosChunkCoords.y, 2) > m_chunkRadius * m_chunkRadius) {
			Chunk* chunk = m_chunks[i];
			chunk->ReleaseGPU();
			m_loadedChunks.erase(std::to_string(chunk->GetPosition().x) + ":" + std::to_string(chunk->GetPosition().y));
			m_chunks.erase(m_chunks.begin() + i);
			freedChunks.push_back(chunk);
		}
	}

	// Add new chunk if needed
	for (int i = -m_chunkRadius + playerPosChunkCoords.x; i < m_chunkRadius + playerPosChunkCoords.x; i++) {
		for (int j = -m_chunkRadius + playerPosChunkCoords.y; j < m_chunkRadius + playerPosChunkCoords.y; j++) {
			if (pow(i - playerPosChunkCoords.x, 2) + pow(j - playerPosChunkCoords.y, 2) <= m_chunkRadius * m_chunkRadius && m_loadedChunks.count(std::to_string(i) + ":" + std::to_string(j)) == 0) {
				m_chunksToCreateQueue.push(std::async(std::launch::async, CreateChunk, i, j, m_seed));
				m_loadedChunks.insert(std::to_string(i) + ":" + std::to_string(j));
			}
		}
	}

	SynchronizeThreads();
}

void ChunkLoader::ReleaseChunks()
{
	for (Chunk* chunk : m_chunks)
		delete chunk;

	for (Chunk* chunk : freedChunks)
		delete chunk;
}

const std::vector<Chunk*>& ChunkLoader::GetLoadedChunks()
{
	return m_chunks;
}

void ChunkLoader::SynchronizeThreads()
{
	while (m_chunksToCreateQueue.size() > 0 && is_ready(m_chunksToCreateQueue.front())) {
		m_chunks.push_back(m_chunksToCreateQueue.front().get());
		m_chunksToCreateQueue.pop();
		m_chunks.back()->LoadGPU();
	}
}
