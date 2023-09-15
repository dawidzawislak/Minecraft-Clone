#include "Chunk.h"
#include "BlocksDB.h"
#include "BlockTextureManager.h"
#include <iostream>

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 256;
constexpr int CHUNK_DPETH = 16;
constexpr int CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DPETH;

FastNoiseLite Chunk::gen;

int GetXYZIndex(int x, int y, int z)
{
	return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z);
}

Chunk::Chunk()
{
	blocks = new int16_t[CHUNK_VOLUME];
	memset(blocks, 0, sizeof(int16_t) * CHUNK_VOLUME);
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
	TOP = 0,
	FRONT = 1,
	BACK = 2,
	LEFT = 3,
	RIGHT = 4,
	BOTTOM = 5
};

void SetUVsAndFace(UVVertex* verts, BlockType blockID, Side side)
{
	std::string texName = BlocksDB::GetTextures(blockID).side;
	glm::vec3 faceNormal;
	switch (side)
	{
	case TOP:
		texName = BlocksDB::GetTextures(blockID).top;
		faceNormal = glm::vec3(0.0, 1.0, 0.0);
		break;
	case FRONT:
		faceNormal = glm::vec3(0.0, 0.0, 1.0);
		break;
	case BACK:
		faceNormal = glm::vec3(0.0, 0.0, -1.0);
		break;
	case LEFT:
		faceNormal = glm::vec3(-1.0, 0.0, 0.0);
		break;
	case RIGHT:
		faceNormal = glm::vec3(1.0, 0.0, 0.0);
		break;
	case BOTTOM:
		texName = BlocksDB::GetTextures(blockID).bottom;
		faceNormal = glm::vec3(0.0, -1.0, 0.0);
		break;
	}

	verts[0].uvs = BlockTextureManager::GetUVLU(texName);
	verts[1].uvs = BlockTextureManager::GetUVLD(texName);
	verts[2].uvs = BlockTextureManager::GetUVRD(texName);
	verts[3].uvs = BlockTextureManager::GetUVRU(texName);

	verts[0].face = verts[1].face = verts[2].face = verts[3].face = (uint32_t)side;
}

double noise(double nx, double ny) 
{ 
	return Chunk::gen.GetNoise(nx, ny) / 2.0 + 0.5;
}

void Chunk::Generate(int posX, int posZ, int seed)
{
	m_posX = posX;
	m_posZ = posZ;

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
		for (int z = 0; z < CHUNK_DPETH; z++) {
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				int xGlobal = x + m_posX * 16 + seed;
				int zGlobal = z + m_posZ * 16 + seed;
				float e = 1 * noise(0.5 * xGlobal, 0.5 * zGlobal);
				+0.5 * noise(1 * xGlobal, 1 * zGlobal);
				+0.5 * noise(2 * xGlobal, 2 * zGlobal);
				float fMaxHeight = e / (2.0) * 255;

				int maxHeight = (int)fMaxHeight;

				if (y == maxHeight)
					blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::STONE;

				else if (y < maxHeight && y > maxHeight - 10)
					blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::DIRT;
				
				else if (y <= maxHeight - 10)
					blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::STONE;
			}
		}
	}

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_DPETH; z++) {
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				if (blocks[GetXYZIndex(x, y, z)] == 0) continue;
				glm::vec3 offset(x, y, z);
				UVVertex faceVerts[4];
				int indicesOffset = renderData.vertices.size();

				// Front Face
				if (z == 15 || (z < 15 && blocks[GetXYZIndex(x, y, z + 1)] == 0)) {
					faceVerts[0].pos = cubeVerts[0] + chunkOrigin + offset;
					faceVerts[1].pos = cubeVerts[1] + chunkOrigin + offset;
					faceVerts[2].pos = cubeVerts[2] + chunkOrigin + offset;
					faceVerts[3].pos = cubeVerts[3] + chunkOrigin + offset;
					SetUVsAndFace(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], FRONT);
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
					SetUVsAndFace(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], BACK);
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
					SetUVsAndFace(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], LEFT);
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
					SetUVsAndFace(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], RIGHT);
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
					SetUVsAndFace(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], TOP);
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
					SetUVsAndFace(faceVerts, (BlockType)blocks[GetXYZIndex(x, y, z)], BOTTOM);
					for (const UVVertex& v : faceVerts)
						renderData.vertices.push_back(v);

					PushIndices(indicesOffset, renderData.indices);
				}
			}
		}
	}
}
