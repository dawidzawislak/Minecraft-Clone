#pragma once

#include <glm/glm.hpp>

enum class BlockType
{
	AIR = 0,
	GRASS,
	DIRT,
	STONE
};

class Block
{
private:
	BlockType m_type;
	float m_xPos, m_yPos, m_zPos;

public:
	Block(BlockType type, float xPos, float yPos, float zPos)
		: m_type(type), m_xPos(xPos), m_yPos(yPos), m_zPos(zPos)
	{}

	~Block() {};

	BlockType GetType() const { return m_type; }
	glm::vec3 GetPosVec3() const { return glm::vec3(m_xPos, m_yPos, m_zPos); }
};