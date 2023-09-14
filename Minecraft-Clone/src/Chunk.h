#pragma once

#include "Block.h"
#include <unordered_map>

struct UVVertex
{
	glm::vec3 pos;
	glm::vec2 uvs;
};

struct ChunkRenderData
{
	std::vector<UVVertex> vertices;
	std::vector<uint32_t> indices;
};

class Chunk
{
public:
	int16_t* blocks;

	ChunkRenderData renderData;

private:
	int m_posX;
	int m_posZ;

public:
	Chunk(int posX, int posZ);
	~Chunk();

	void Generate();
};