#include "BlockTextureManager.h"
#include "Renderer.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <vector>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

std::unordered_map<std::string, glm::vec2> BlockTextureManager::m_uvLU;
std::unordered_map<std::string, glm::vec2> BlockTextureManager::m_uvRD;

std::vector<Pixel> BlockTextureManager::m_data;
unsigned int BlockTextureManager::m_width = 0, BlockTextureManager::m_height = 0, BlockTextureManager::m_bpp = 0;

unsigned int BlockTextureManager::m_textureAtlasID;

bool BlockTextureManager::Initialize(std::string blockTexturesDir)
{
	int count = 0;
	for (const auto& dir_entry : fs::directory_iterator(blockTexturesDir))
		if (dir_entry.path().extension().string() == ".png") count++;

	int textureAtlasWidth = 16 * (int)sqrt(count);

	unsigned int offX, offY, lineHeight;
	offX = offY = lineHeight = 0u;

	for (const auto& dir_entry : fs::directory_iterator(blockTexturesDir)) {
		if (dir_entry.path().extension().string() != ".png") continue;

		int w, h, channels;
		int8_t* data = (int8_t*)stbi_load(dir_entry.path().string().c_str(), &w, &h, &channels, 4);

		if (offX + w > textureAtlasWidth) {
			offX = 0;
			offY += lineHeight;
			lineHeight = 0;
		}

		if (lineHeight < h) {
			lineHeight = h;
			m_data.resize(textureAtlasWidth * (lineHeight + offY) * 4);
		}

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				unsigned int globalX, globalY;
				globalX = offX + x;
				globalY = offY + y;
				m_data[globalY * textureAtlasWidth + globalX].r = data[(y * w + x) * 4];
				m_data[globalY * textureAtlasWidth + globalX].g = data[(y * w + x) * 4 + 1];
				m_data[globalY * textureAtlasWidth + globalX].b = data[(y * w + x) * 4 + 2];
				m_data[globalY * textureAtlasWidth + globalX].a = data[(y * w + x) * 4 + 3];
			}
		}
		std::string fileName = dir_entry.path().filename().string();
		fileName = fileName.substr(0, fileName.length() - 4);

		float LUu = offX / (float)textureAtlasWidth;
		float RDu = (offX + w) / (float)textureAtlasWidth;
		m_uvLU[fileName] = glm::vec2(LUu, offY);
		m_uvRD[fileName] = glm::vec2(RDu, offY+h);

		offX += w;
	}

	unsigned int textureAtlasHeight = offY + lineHeight;

	for (auto it = m_uvLU.begin(); it != m_uvLU.end(); it++)
		it->second.y /= (float)textureAtlasHeight;

	for (auto it = m_uvRD.begin(); it != m_uvRD.end(); it++)
		it->second.y /= (float)textureAtlasHeight;

	GLCall(glGenTextures(1, &m_textureAtlasID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureAtlasID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureAtlasWidth, textureAtlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)m_data.data()));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//stbi_write_png("textureAtlas.png", textureAtlasWidth, offY+lineHeight, 4, m_data.data(), sizeof(Pixel) * textureAtlasWidth);
	if (m_data.size() > 0) {
		m_data.clear();
		m_data.shrink_to_fit();
	}

	return true;
}

void BlockTextureManager::Release()
{
	GLCall(glDeleteTextures(1, &m_textureAtlasID));
}

void BlockTextureManager::BindTextureAtlas(int slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureAtlasID));
}

const glm::vec2& BlockTextureManager::GetUVLU(std::string textureName)
{
	return m_uvLU[textureName];
}

glm::vec2 BlockTextureManager::GetUVLD(std::string textureName)
{
	return glm::vec2(m_uvLU[textureName].x, m_uvRD[textureName].y);
}

const glm::vec2& BlockTextureManager::GetUVRD(std::string textureName)
{
	return m_uvRD[textureName];
}

glm::vec2 BlockTextureManager::GetUVRU(std::string textureName)
{
	return glm::vec2(m_uvRD[textureName].x, m_uvLU[textureName].y);
}