#pragma once

#include "Block.h"
#include <unordered_map>

class Chunk
{
private:
	BlockType m_blocks[16][16][256];
	std::unordered_map<std::string, int> m_blocksToRender;
	int m_xPos, m_zPos;

public:
	Chunk(int x, int y);
	~Chunk();

	BlockType GetBlockType(unsigned int x, unsigned int y, unsigned int z) const { return m_blocks[x][z][y]; }
	const std::unordered_map<std::string, int>& BlocksToRender() const { return m_blocksToRender; }
	glm::vec3 GetPosVec3() const { return glm::vec3(m_xPos, -256.0f, m_zPos); }
};