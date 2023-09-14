#pragma once

#include <unordered_map>

enum class BlockType
{
	AIR = 0,
	GRASS = 1,
	SAND = 2
};

struct TextureNames
{
	std::string top;
	std::string side;
	std::string bottom;
};

class BlocksDB
{
	static std::unordered_map<int16_t, TextureNames> m_textureNames;

private:
	BlocksDB();

public:
	static void Initialize();

	static TextureNames GetTextures(BlockType blockID) { return m_textureNames[(int16_t)blockID]; }
};