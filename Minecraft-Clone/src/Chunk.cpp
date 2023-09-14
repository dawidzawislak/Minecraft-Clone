#include "Chunk.h"

#include <string>



/*

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
*/


constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 256;
constexpr int CHUNK_DPETH = 16;
constexpr int CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DPETH;

int GetXYZIndex(int x, int y, int z)
{
	return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z);
}

Chunk::Chunk(int posX, int posZ)
{
	m_posX = posX;
	m_posZ = posZ;

	blocks = new int16_t[CHUNK_VOLUME];
	memset(blocks, 1, sizeof(int16_t) * CHUNK_VOLUME);
}

Chunk::~Chunk()
{
	delete[] blocks;
}

void PushIndices(int vertOffset, std::vector<uint32_t>& indices)
{
	indices.push_back(vertOffset + 0);
	indices.push_back(vertOffset + 1);
	indices.push_back(vertOffset + 3);
	indices.push_back(vertOffset + 1);
	indices.push_back(vertOffset + 2);
	indices.push_back(vertOffset + 3);
}

void Chunk::Generate()
{
	glm::vec3 chunkOrigin(m_posX * 16, 0, m_posZ * 16);

	glm::vec3 cubeVerts[8];
	cubeVerts[0] = glm::vec3(0.0f, 1.0f, 0.0f); // ful
	cubeVerts[1] = glm::vec3(0.0f, 0.0f, 0.0f); // fll
	cubeVerts[2] = glm::vec3(1.0f, 0.0f, 0.0f); // flr
	cubeVerts[3] = glm::vec3(1.0f, 1.0f, 0.0f); // ful

	cubeVerts[4] = glm::vec3(0.0f, 1.0f, -1.0f); // bul
	cubeVerts[5] = glm::vec3(0.0f, 0.0f, -1.0f); // bll
	cubeVerts[6] = glm::vec3(1.0f, 0.0f, -1.0f); // blr
	cubeVerts[7] = glm::vec3(1.0f, 1.0f, -1.0f); // bul

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_DPETH; z++) {
				glm::vec3 offset(x, y, z);
				UVVertex faceVerts[4];
				faceVerts[0].uvs = { 0,1 };
				faceVerts[1].uvs = { 0,0 };
				faceVerts[2].uvs = { 1,0 };
				faceVerts[3].uvs = { 1,1 };
				int indicesOffset = renderData.vertices.size();

				// Front Face
				if (z == 15 || (z < 15 && blocks[GetXYZIndex(x, y, z + 1)] == 0)) {
					faceVerts[0].pos = cubeVerts[0] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[1] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[2] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[3] + chunkOrigin + offset;

					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
					indicesOffset += 4;
				}
				// Back Face
				if (z == 0 || (z > 0 && blocks[GetXYZIndex(x, y, z - 1)] == 0)) {
					faceVerts[0].pos = cubeVerts[7] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[6] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[5] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[4] + chunkOrigin + offset;

					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
					indicesOffset += 4;
				}

				// Left Face
				if (x == 0 || (x > 0 && blocks[GetXYZIndex(x - 1, y, z)] == 0)) {
					faceVerts[0].pos = cubeVerts[4] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[5] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[1] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[0] + chunkOrigin + offset;

					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
					indicesOffset += 4;
				}
				// Right Face
				if (x == 15 || (x < 15 && blocks[GetXYZIndex(x + 1, y, z)] == 0)) {
					faceVerts[0].pos = cubeVerts[3] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[2] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[6] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[7] + chunkOrigin + offset;

					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
					indicesOffset += 4;
				}

				// Up Face
				if (y == 255 || (x < 255 && blocks[GetXYZIndex(x, y + 1, z)] == 0)) {
					faceVerts[0].pos = cubeVerts[4] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[0] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[3] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[7] + chunkOrigin + offset;

					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
					indicesOffset += 4;
				}

				// Bottom Face
				if (y == 0 || (x > 0 && blocks[GetXYZIndex(x, y - 1, z)] == 0)) {
					faceVerts[0].pos = cubeVerts[1] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[5] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[6] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[2] + chunkOrigin + offset;

					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
				}
			}
		}
	}
}
