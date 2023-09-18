#pragma once

#include <vector>
#include <queue>
#include <unordered_set>
#include <future>
#include "Chunk.h"

class ChunkLoader
{
public:
	std::vector<Chunk> loadedChunks;

private:
	uint32_t m_chunkRadius;

	std::unordered_set<std::string> m_loadedChunks;

	std::queue<std::future<Chunk*>> m_chunksToCreateQueue;

public:
	void Initialize(uint32_t chunkRadius, glm::ivec2 playerPos);

	void Update(glm::ivec2 playerPos);
};