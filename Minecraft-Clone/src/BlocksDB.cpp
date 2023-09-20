#include "BlocksDB.h"

std::vector<TextureNames> BlocksDB::m_textureNames;

void BlocksDB::Initialize()
{
	m_textureNames.resize((int16_t)BlockType::SIZE_OF_STRUCTURE);
	// { Top, Side, Bottom }
	m_textureNames[(int16_t)BlockType::GRASS] = { "grass_block_top", "grass_block_side", "dirt" };
	m_textureNames[(int16_t)BlockType::DIRT] = { "dirt", "dirt", "dirt" };
	m_textureNames[(int16_t)BlockType::STONE] = { "stone", "stone", "stone" };
	m_textureNames[(int16_t)BlockType::SAND] = { "sand", "sand", "sand" };
	m_textureNames[(int16_t)BlockType::TERRACOTTA_LIME] = { "lime_terracotta", "lime_terracotta", "lime_terracotta" };
}
