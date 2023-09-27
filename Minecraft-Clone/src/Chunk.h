#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "FastNoiseLite/FastNoiseLite.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

enum class BlockFace
{
	TOP = 0,
	FRONT = 1,
	BACK = 2,
	LEFT = 3,
	RIGHT = 4,
	BOTTOM = 5
};

struct UVVertex
{
	uint32_t data1; // position(17bits), blockFace(3bits), textureID(12bits)
	uint32_t data2; // whichVertex(on face - 2bits)
};

struct ChunkRenderData
{
	std::vector<UVVertex> vertices;
	std::vector<uint32_t> indices;

	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
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
	int m_seed;

public:
	Chunk();
	~Chunk();

	void SetChunkData(int posX, int posZ, int seed);
	void CreateRenderData();
	
	void LoadCPU(int posX, int posZ, int seed);
	void LoadGPU();

	void ReleaseGPU();

	bool SaveChunkDataToFile(const std::string& folderDirectory);
	bool LoadChunkDataFromFile(const std::string& folderDirectory);

	int GetXYZIndex(int x, int y, int z);

	glm::ivec2 GetPosition() const { return glm::ivec2(m_posX, m_posZ); }
};