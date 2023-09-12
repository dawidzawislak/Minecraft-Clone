#include "Chunk.h"

#include <string>

Chunk::Chunk(int x, int y, float* heights)
{
	m_xPos = x * 16.0f;
	m_zPos = y * 16.0f;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			int airStarts = heights[x*16+z] * 256;
			
			for (int y = 0; y < airStarts - 4; y++)
				m_blocks[x][z][y] = BlockType::STONE;

			for (int y = airStarts - 4; y < airStarts - 1; y++)
				m_blocks[x][z][y] = BlockType::DIRT;

			m_blocks[x][z][airStarts - 1] = BlockType::GRASS;

			for (int y = airStarts; y < 256; y++)
				m_blocks[x][z][y] = BlockType::AIR;

		}
	}
}

void Chunk::Initialize(int x, int y, float* heights)
{
	m_xPos = x * 16.0f;
	m_zPos = y * 16.0f;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			int airStarts = heights[x * 16 + z] * 256;

			for (int y = 0; y < airStarts - 4; y++)
				m_blocks[x][z][y] = BlockType::STONE;

			for (int y = airStarts - 4; y < airStarts - 1; y++)
				m_blocks[x][z][y] = BlockType::DIRT;

			m_blocks[x][z][airStarts - 1] = BlockType::GRASS;

			for (int y = airStarts; y < 256; y++)
				m_blocks[x][z][y] = BlockType::AIR;

		}
	}
}

void Chunk::Update(Chunk* xmin1Chunk, const Chunk* xplus1Chunk, Chunk* zmin1Chunk, const Chunk* zplus1Chunk)
{
	for (int x = 0; x < 16; x++)
		for (int z = 0; z < 16; z++)
			for (int y = 0; y < 256; y++) {
				if (m_blocks[x][z][y] == BlockType::AIR) {
					// left block
					if (x > 0 && m_blocks[x - 1][z][y] != BlockType::AIR) {
						std::string index = std::to_string(x - 1) + ";" + std::to_string(y) + ";" + std::to_string(z);
						if (!m_blocksToRender.count(index))
							m_blocksToRender[index] = 0x100;
						else
							m_blocksToRender[index] += 0x100;
					}
					// right block
					if (x < 15 && m_blocks[x + 1][z][y] != BlockType::AIR) {
						std::string index = std::to_string(x + 1) + ";" + std::to_string(y) + ";" + std::to_string(z);
						if (!m_blocksToRender.count(index))
							m_blocksToRender[index] = 0x1;
						else
							m_blocksToRender[index] += 0x1;
					}

					//front block
					if (z > 0 && m_blocks[x][z - 1][y] != BlockType::AIR) {
						std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z - 1);
						if (!m_blocksToRender.count(index))
							m_blocksToRender[index] = 0x10;
						else
							m_blocksToRender[index] += 0x10;
					}

					// back block
					if (z < 15 && m_blocks[x][z + 1][y] != BlockType::AIR) {
						std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z + 1);
						if (!m_blocksToRender.count(index))
							m_blocksToRender[index] = 0x1000;
						else
							m_blocksToRender[index] += 0x1000;
					}

					// bottom block
					if (y > 0 && m_blocks[x][z][y - 1] != BlockType::AIR) {
						std::string index = std::to_string(x) + ";" + std::to_string(y - 1) + ";" + std::to_string(z);
						if (!m_blocksToRender.count(index))
							m_blocksToRender[index] = 0x100000;
						else
							m_blocksToRender[index] += 0x100000;
					}

					// top block
					if (y < 255 && m_blocks[x][z][y + 1] != BlockType::AIR) {
						std::string index = std::to_string(x) + ";" + std::to_string(y + 1) + ";" + std::to_string(z);
						if (!m_blocksToRender.count(index))
							m_blocksToRender[index] = 0x10000;
						else
							m_blocksToRender[index] += 0x10000;
					}
				}
				else {
					if (x == 0 && xmin1Chunk)
					{
						if (xmin1Chunk->GetBlockType(15, y, z) == BlockType::AIR) {
							std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);
							if (!m_blocksToRender.count(index))
								m_blocksToRender[index] = 0x1;
							else
								m_blocksToRender[index] += 0x1;
						}
					}
					else if (x == 15 && xplus1Chunk)
					{
						if (xplus1Chunk->GetBlockType(0, y, z) == BlockType::AIR) {
							std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);
							if (!m_blocksToRender.count(index))
								m_blocksToRender[index] = 0x100;
							else
								m_blocksToRender[index] += 0x100;
						}
					}

					if (z == 0 && zmin1Chunk)
					{
						if (zmin1Chunk->GetBlockType(x, y, 15) == BlockType::AIR) {
							std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);
							if (!m_blocksToRender.count(index))
								m_blocksToRender[index] = 0x1000;
							else
								m_blocksToRender[index] += 0x1000;
						}
					}
					else if (z == 15 && zplus1Chunk)
					{
						if (zplus1Chunk->GetBlockType(x, y, 0) == BlockType::AIR) {
							std::string index = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);
							if (!m_blocksToRender.count(index))
								m_blocksToRender[index] = 0x10;
							else
								m_blocksToRender[index] += 0x10;
						}
					}
				}
			}
}

Chunk::~Chunk()
{
}
