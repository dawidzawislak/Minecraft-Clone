#pragma once

#include <vector>
#include <queue>
#include <unordered_set>
#include <future>
#include "Chunk.h"

class ChunkLoader
{
private:
	static std::vector<Chunk*> m_chunks;

	static int m_chunkRadius;
	static int m_seed;

	static std::unordered_set<std::string> m_loadedChunks;
	static std::queue<std::future<Chunk*>> m_chunksToCreateQueue;

public:
	static void Initialize(uint32_t chunkRadius, glm::vec3 playerPos, int seed);

	static void Update(glm::vec3 playerPos);

	static void ReleaseChunks();

	static const std::vector<Chunk*>& GetLoadedChunks();

private:
	ChunkLoader();

	static void SynchronizeThreads();
};