#pragma once

#include <unordered_map>

#include "Block.h"
#include "Texture.h"

class BlockTextureManager
{
	std::unordered_map<BlockType, Texture*> m_tops;
	std::unordered_map<BlockType, Texture*> m_sides;
	std::unordered_map<BlockType, Texture*> m_bottoms;

public:
	bool Initialize(const std::string& filePath);
	void Release();

	void BindSideTexture(BlockType type);
	void BindTopTexture(BlockType type);
	void BindBottomTexture(BlockType type);
};