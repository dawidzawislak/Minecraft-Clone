#include "Chunk.h"

Chunk::Chunk()
{
	m_xPos = 0;
	m_zPos = 0;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int y = 0; y < 196; y++)
				m_blocks[x][z][y] = BlockType::STONE;

			for (int y = 196; y < 199; y++)
				m_blocks[x][z][y] = BlockType::DIRT;

			m_blocks[x][z][199] = BlockType::GRASS;

			for (int y = 200; y < 256; y++)
				m_blocks[x][z][y] = BlockType::AIR;
		}
	}

	for (int x = 1; x < 15; x++)
		for (int z = 1; z < 15; z++)
			for (int y = 1; y < 255; y++)
				if (m_blocks[x - 1][z][y] == BlockType::AIR || m_blocks[x + 1][z][y] == BlockType::AIR ||
					m_blocks[x][z - 1][y] == BlockType::AIR || m_blocks[x][z + 1][y] == BlockType::AIR ||
					m_blocks[x][z][y - 1] == BlockType::AIR || m_blocks[x][z][y + 1] == BlockType::AIR)
					m_blocksToRender.emplace_back(x, y, z);
}

Chunk::~Chunk()
{
}
