#include "Chunk.h"

#include <string>

Chunk::Chunk(int x, int y)
{
	m_xPos = x * 16.0f;
	m_zPos = y * 16.0f;

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

	m_blocks[10][10][200] = BlockType::GRASS;
	m_blocks[10][11][200] = BlockType::GRASS;
	m_blocks[11][11][200] = BlockType::GRASS;
	m_blocks[11][10][200] = BlockType::GRASS;

	for (int x = 0; x < 16; x++)
		for (int z = 0; z < 16; z++)
			for (int y = 0; y < 256; y++) {
				if (m_blocks[x][z][y] == BlockType::AIR) continue;
				std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);
				if (x > 0 && m_blocks[x - 1][z][y] == BlockType::AIR) {
					if (!m_blocksToRender.count(index))
						m_blocksToRender[index] = 0x1;
					else
						m_blocksToRender[index] += 0x1;
				}
				if (x < 15 && m_blocks[x + 1][z][y] == BlockType::AIR) {
					if (!m_blocksToRender.count(index))
						m_blocksToRender[index] = 0x100;
					else
						m_blocksToRender[index] += 0x100;
				}

				if (z > 0 && m_blocks[x][z - 1][y] == BlockType::AIR) {
					if (!m_blocksToRender.count(index))
						m_blocksToRender[index] = 0x1000;
					else
						m_blocksToRender[index] += 0x1000;
				}
				if (z < 15 && m_blocks[x][z + 1][y] == BlockType::AIR) {
					if (!m_blocksToRender.count(index))
						m_blocksToRender[index] = 0x10;
					else
						m_blocksToRender[index] += 0x10;
				}
				if (y > 0 && m_blocks[x][z][y - 1] == BlockType::AIR) {
					if (!m_blocksToRender.count(index))
						m_blocksToRender[index] = 0x10000;
					else
						m_blocksToRender[index] += 0x10000;
				}
				if (y < 256 && m_blocks[x][z][y + 1] == BlockType::AIR) {
					if (!m_blocksToRender.count(index))
						m_blocksToRender[index] = 0x100000;
					else
						m_blocksToRender[index] += 0x100000;
				}
			}
}

Chunk::~Chunk()
{
}
