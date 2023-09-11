#include "BlockTextureManager.h"

#include "stb_image/stb_image.h"

bool BlockTextureManager::Initialize(const std::string& filePath)
{
	stbi_set_flip_vertically_on_load(1);
	int w, h, bpp;
	unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &bpp, 4);

	m_tops[BlockType::GRASS] = new Texture(&data[32 * 16 * 4], 16, 16, bpp);
	m_sides[BlockType::GRASS] = new Texture(&data[16 * 16 * 4], 16, 16, bpp);
	m_bottoms[BlockType::GRASS] = new Texture(data, 16, 16, bpp);

	if (data)
		stbi_image_free(data);
	else
		return false;

	return true;
}

void BlockTextureManager::Release()
{
	delete m_tops[BlockType::GRASS];
	delete m_sides[BlockType::GRASS];
	delete m_bottoms[BlockType::GRASS];
}

void BlockTextureManager::BindSideTexture(BlockType type)
{
	m_sides[type]->Bind();
}
void BlockTextureManager::BindTopTexture(BlockType type)
{
	m_tops[type]->Bind();
}
void BlockTextureManager::BindBottomTexture(BlockType type)
{
	m_bottoms[type]->Bind();
}