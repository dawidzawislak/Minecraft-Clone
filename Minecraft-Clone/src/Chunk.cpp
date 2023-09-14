#include "Chunk.h"
#include "BlocksDB.h"
#include "BlockTextureManager.h"

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
	memset(blocks, 0, sizeof(int16_t) * CHUNK_VOLUME);

	for (int x = 0; x < CHUNK_WIDTH; x++)
		for (int z = 0; z < CHUNK_DPETH; z++)
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				if (y >= 30) blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::GRASS;
				else blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::SAND;
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

enum Side
{
	TOP,
	SIDE,
	BOTTOM
};

void SetUVs(UVVertex* uvs, BlockType blockID, Side side)
{
	std::string texName = "";
	switch (side)
	{
	case TOP:
		texName = BlocksDB::GetTextures(blockID).top;
		break;
	case SIDE:
		texName = BlocksDB::GetTextures(blockID).side;
		break;
	case BOTTOM:
		texName = BlocksDB::GetTextures(blockID).bottom;
		break;
	}

	uvs[0].uvs = BlockTextureManager::GetUVLU(texName);
	uvs[1].uvs = BlockTextureManager::GetUVLD(texName);
	uvs[2].uvs = BlockTextureManager::GetUVRD(texName);
	uvs[3].uvs = BlockTextureManager::GetUVRU(texName);
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
				int indicesOffset = renderData.vertices.size();

				// Front Face
				if (z == 15 || (z < 15 && blocks[GetXYZIndex(x, y, z + 1)] == 0)) {
					faceVerts[0].pos = cubeVerts[0] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[1] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[2] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[3] + chunkOrigin + offset;
					SetUVs(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], SIDE);
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
					SetUVs(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], SIDE);
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
					SetUVs(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], SIDE);
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
					SetUVs(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], SIDE);
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
					SetUVs(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], TOP);
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
					SetUVs(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], BOTTOM);
					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
				}
			}
		}
	}
}
