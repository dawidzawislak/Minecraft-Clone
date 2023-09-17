#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "FastNoiseLite/FastNoiseLite.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
//#include <future>

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
/*
enum class LoadingState
{
	NOT_LOADED,
	LOADED_CPU,
	FULLY_LOADED
};
*/
class Chunk
{
public:
	int16_t* blocks;

	ChunkRenderData renderData;

	static FastNoiseLite gen;

	bool loaded;

private:
	int m_posX;
	int m_posZ;

	//std::vector<std::future<void>> m_futures;

public:
	Chunk();
	~Chunk();

	void SetChunkData(int posX, int posZ, int seed);
	void CreateRenderData();
	/*
	void LoadCPU();
	void LoadGPU();

	void Load();*/
	void Release();

	glm::ivec2 GetPosition() const { return glm::ivec2(m_posX, m_posZ); }
};