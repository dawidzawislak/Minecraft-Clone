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
	static std::unordered_map<std::string, glm::vec2> m_uvLU;
	static std::unordered_map<std::string, glm::vec2> m_uvRD;

	static std::vector<Pixel> m_data;
	static unsigned int m_width, m_height, m_bpp;

	static unsigned int m_textureAtlasID;

private:
	BlockTextureManager() {};

public:
	static bool Initialize(std::string blockTexturesDir);
	static void Release();

	static void BindTextureAtlas(int slot = 0);

	static const glm::vec2& GetUVLU(std::string textureName);
	static glm::vec2 GetUVLD(std::string textureName);
	static const glm::vec2& GetUVRD(std::string textureName);
	static glm::vec2 GetUVRU(std::string textureName);
};