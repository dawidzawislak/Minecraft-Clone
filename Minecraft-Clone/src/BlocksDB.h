#pragma once

#include <unordered_map>
#include <string>

enum class BlockType
{
	AIR = 0,
	GRASS,
	DIRT,
	STONE,
	SAND,
	TERRACOTTA_LIME,
	SIZE_OF_STRUCTURE
};

struct TextureNames
{
	std::string top;
	std::string side;
	std::string bottom;
};

class BlocksDB
{
	static std::vector<TextureNames> m_textureNames;

private:
	BlocksDB();

public:
	static void Initialize();

	static const TextureNames& GetTextures(BlockType blockID) { return m_textureNames[(int16_t)blockID]; }
};