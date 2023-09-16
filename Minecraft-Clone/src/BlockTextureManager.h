#pragma once

#include <unordered_map>
#include "BlocksDB.h"
#include <glm/glm.hpp>

struct Pixel
{
	int8_t r, g, b, a;
};

class BlockTextureManager
{
	static std::vector<float> m_UVs;
	static std::unordered_map<std::string, uint16_t> m_textureIDs;

	static unsigned int m_textureAtlasID;

	static unsigned int m_UVTextureBufferID;
	static unsigned int m_UVTextureID;

private:
	BlockTextureManager() {};

public:
	static bool Initialize(std::string blockTexturesDir);
	static void Release();

	static void BindTextureAtlasAndUVTextureBuffer();

	static uint16_t GetTextureID(const std::string& name);
};