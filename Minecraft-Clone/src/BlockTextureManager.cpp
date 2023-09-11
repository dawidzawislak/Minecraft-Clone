#include "BlockTextureManager.h"

#include "stb_image/stb_image.h"

bool BlockTextureManager::Initialize(std::string fileNamesTemplate, int count)
{
	stbi_set_flip_vertically_on_load(1);

	int posToChange = fileNamesTemplate.find('_');

	for (int i = 1; i <= count; i++) {
		std::string x = std::to_string(i);
		fileNamesTemplate.replace(posToChange, 1, std::to_string(i));
		
		int w, h, bpp;
		unsigned char* data = stbi_load(fileNamesTemplate.c_str(), &w, &h, &bpp, 4);

		m_tops[(BlockType)i] = new Texture(&data[32 * 16 * 4], 16, 16, bpp);
		m_sides[(BlockType)i] = new Texture(&data[16 * 16 * 4], 16, 16, bpp);
		m_bottoms[(BlockType)i] = new Texture(data, 16, 16, bpp);
		if (data)
			stbi_image_free(data);
		else
			return false;
	}

	return true;
}

void BlockTextureManager::Release()
{

	for (auto i = m_tops.begin(); i != m_tops.end(); i++)
		delete i->second;

	for (auto i = m_sides.begin(); i != m_sides.end(); i++)
		delete i->second;

	for (auto i = m_bottoms.begin(); i != m_bottoms.end(); i++)
		delete i->second;
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