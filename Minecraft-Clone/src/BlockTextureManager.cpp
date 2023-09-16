#include "BlockTextureManager.h"
#include "Renderer.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <vector>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<float> BlockTextureManager::m_UVs;
std::unordered_map<std::string, uint16_t> BlockTextureManager::m_textureIDs;

unsigned int BlockTextureManager::m_textureAtlasID;

unsigned int BlockTextureManager::m_UVTextureBufferID;
unsigned int BlockTextureManager::m_UVTextureID;

bool BlockTextureManager::Initialize(std::string blockTexturesDir)
{
	std::vector<Pixel> m_data;

	int count = 0;
	for (const auto& dir_entry : fs::directory_iterator(blockTexturesDir))
		if (dir_entry.path().extension().string() == ".png") count++;

	int textureAtlasWidth = 16 * (int)sqrt(count);

	uint32_t offX, offY, lineHeight;
	offX = offY = lineHeight = 0u;

	uint32_t textureID = 0;

	std::vector<glm::vec2> uvLU;
	std::vector<glm::vec2> uvRD;

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
		uvLU.push_back(glm::vec2(LUu, offY));
		uvRD.push_back(glm::vec2(RDu, offY + h));
		m_textureIDs[fileName] = textureID;

		textureID++;
		offX += w;
	}

	uint32_t textureAtlasHeight = offY + lineHeight;

	for (int i = 0; i < textureID; i++) {
		uvLU[i].y /= (float)textureAtlasHeight;
		uvRD[i].y /= (float)textureAtlasHeight;

		// LU
		m_UVs.push_back(uvLU[i].x);
		m_UVs.push_back(uvLU[i].y);

		// LD
		m_UVs.push_back(uvLU[i].x);
		m_UVs.push_back(uvRD[i].y);

		// RD
		m_UVs.push_back(uvRD[i].x);
		m_UVs.push_back(uvRD[i].y);

		// RU
		m_UVs.push_back(uvRD[i].x);
		m_UVs.push_back(uvLU[i].y);
	}

	// Initializing UV Texture
	GLCall(glGenBuffers(1, &m_UVTextureBufferID));
	GLCall(glBindBuffer(GL_TEXTURE_BUFFER, m_UVTextureBufferID));
	GLCall(glBufferData(GL_TEXTURE_BUFFER, m_UVs.size() * sizeof(float), (const void*)m_UVs.data(), GL_STATIC_DRAW));

	GLCall(glGenTextures(1, &m_UVTextureID));
	
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
	// END: Initializing UV Texture

	// Initializing Texture atlas
	GLCall(glGenTextures(1, &m_textureAtlasID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureAtlasID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureAtlasWidth, textureAtlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)m_data.data()));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	// END: Initializing Texture atlas

	//stbi_write_png("textureAtlas.png", textureAtlasWidth, offY+lineHeight, 4, m_data.data(), sizeof(Pixel) * textureAtlasWidth);

	return true;
}

void BlockTextureManager::Release()
{
	GLCall(glDeleteTextures(1, &m_textureAtlasID));

	GLCall(glDeleteBuffers(1, &m_UVTextureBufferID));
	GLCall(glDeleteTextures(1, &m_UVTextureID));
}

void BlockTextureManager::BindTextureAtlasAndUVTextureBuffer()
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureAtlasID));

	GLCall(glActiveTexture(GL_TEXTURE1));
	GLCall(glBindTexture(GL_TEXTURE_BUFFER, m_UVTextureID));
	GLCall(glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, m_UVTextureBufferID));
}

uint16_t BlockTextureManager::GetTextureID(const std::string& name)
{
	return m_textureIDs[name];
}