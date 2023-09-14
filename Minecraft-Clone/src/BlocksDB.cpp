#include "BlocksDB.h"

std::unordered_map<int16_t, TextureNames> BlocksDB::m_textureNames;

void BlocksDB::Initialize()
{
	// { Top, Side, Bottom }
	m_textureNames[(int16_t)BlockType::GRASS] = { "grass_block_top", "grass_block_side", "dirt" };
	m_textureNames[(int16_t)BlockType::SAND] = { "sand", "sand", "sand" };
}
