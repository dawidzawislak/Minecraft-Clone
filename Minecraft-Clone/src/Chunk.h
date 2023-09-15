#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "FastNoiseLite/FastNoiseLite.h"

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

	static FastNoiseLite gen;

private:
	int m_posX;
	int m_posZ;

public:
	Chunk();
	~Chunk();

	void Generate(int posX, int posZ, int seed);
};