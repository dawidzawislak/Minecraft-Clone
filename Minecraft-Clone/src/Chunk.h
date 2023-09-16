#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "FastNoiseLite/FastNoiseLite.h"

struct UVVertex
{
	uint32_t data1; // position(17bits), blockFace(3bits), textureID(12bits)
	uint32_t data2; // whichVertex(on face - 2bits)
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

	glm::ivec2 GetPosition() const { return glm::ivec2(m_posX, m_posZ); }
};