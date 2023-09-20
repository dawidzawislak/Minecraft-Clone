#include "Chunk.h"
#include "BlocksDB.h"
#include "BlockTextureManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 256;
constexpr int CHUNK_DEPTH = 16;
constexpr int CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

FastNoiseLite Chunk::gen;

constexpr uint32_t MASK_POS = 0x1FFFF;
constexpr uint32_t MASK_FACE = 0xE0000;

enum class BlockFace
{
	TOP = 0,
	FRONT = 1,
	BACK = 2,
	LEFT = 3,
	RIGHT = 4,
	BOTTOM = 5
};

enum class VertexPos
{
	LU = 0,
	LD = 1,
	RD = 2,
	RU = 3
};

void SetVertexData(UVVertex& vertex, glm::uvec3 pos, BlockFace face, BlockType blockID, VertexPos vertexPos)
{
	vertex.data1 = 0u;
	vertex.data1 += pos.x;
	vertex.data1 += pos.y * 17;
	vertex.data1 += pos.z * 17 * 17 * 17;

	vertex.data1 |= ((uint32_t)face % 6) << 17;

	const TextureNames& texNames = BlocksDB::GetTextures(blockID);
	uint16_t textureID = BlockTextureManager::GetTextureID(texNames.side);

	switch (face)
	{
	case BlockFace::TOP:
		textureID = BlockTextureManager::GetTextureID(texNames.top);
		break;
	case BlockFace::BOTTOM:
		textureID = BlockTextureManager::GetTextureID(texNames.bottom);
		break;
	}

	vertex.data1 |= ((uint32_t)textureID) << 20;

	vertex.data2 = (uint32_t)vertexPos;
}

int Chunk::GetXYZIndex(int x, int y, int z)
{
	return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z);
}

Chunk::Chunk()
{
	m_posX = m_posZ = m_seed = 0;
	blocks = new int16_t[CHUNK_VOLUME];
}

Chunk::~Chunk()
{
	delete[] blocks;
}

void PushIndices(std::vector<uint32_t>& indices, int vertOffset)
{
	indices.push_back(vertOffset + 0);
	indices.push_back(vertOffset + 1);
	indices.push_back(vertOffset + 3);
	indices.push_back(vertOffset + 1);
	indices.push_back(vertOffset + 2);
	indices.push_back(vertOffset + 3);
}

void PushVertices(std::vector<UVVertex>& vertices, UVVertex* vertsToPush)
{
	vertices.push_back(vertsToPush[0]);
	vertices.push_back(vertsToPush[1]);
	vertices.push_back(vertsToPush[2]);
	vertices.push_back(vertsToPush[3]);
}

double noise(double nx, double ny) 
{ 
	return Chunk::gen.GetNoise(nx, ny) / 2.0 + 0.5;
}

void Chunk::SetChunkData(int posX, int posZ, int seed)
{
	memset(blocks, 0, sizeof(int16_t) * CHUNK_VOLUME);

	m_posX = posX;
	m_posZ = posZ;
	m_seed = seed;
	for (uint32_t x = 0; x < CHUNK_WIDTH; x++) {
		for (uint32_t z = 0; z < CHUNK_DEPTH; z++) {
			for (uint32_t y = 0; y < CHUNK_HEIGHT; y++) {
				int xGlobal = x + m_posX * 16 + seed;
				int zGlobal = z + m_posZ * 16 + seed;
				float e = 1 * noise(0.25 * xGlobal, 0.25 * zGlobal) +0.25 * noise(1 * xGlobal, 1 * zGlobal);
				float fMaxHeight = e / (2.0) * 255;

				int maxHeight = (int)fMaxHeight;

				if (y == maxHeight)
					blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::TERRACOTTA_LIME;

				else if (y < maxHeight && y > maxHeight - 10)
					blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::DIRT;
				
				else if (maxHeight  > 10 && y <= maxHeight - 10)
					blocks[GetXYZIndex(x, y, z)] = (int16_t)BlockType::STONE;
			}
		}
	}
}

bool Chunk::LoadChunkDataFromFile(const std::string& folderDirectory)
{
	std::string fileName = folderDirectory + "/s" + std::to_string(m_seed) + "_" + std::to_string(m_posX) + "_" + std::to_string(m_posZ) + ".chunk";
	std::ifstream fileIn(fileName, std::ios::binary);

	if (fileIn.is_open()) {
		fileIn.read((char*)blocks, sizeof(int16_t) * CHUNK_VOLUME);
		fileIn.close();
		//std::cout << "Read chunk from file <" << m_posX << ", " << m_posZ << ">\n";
		return true;
	}

	std::cout << "Failed to read chunk <" << m_posX << ", " << m_posZ << "> from file\n";
	return false;
}

bool Chunk::SaveChunkDataToFile(const std::string& folderDirectory)
{
	std::string fileName = folderDirectory + "/s" + std::to_string(m_seed) + "_" + std::to_string(m_posX) + "_" + std::to_string(m_posZ) + ".chunk";
	std::ofstream fileOut(fileName, std::ios::binary | std::ios::trunc);
	if (!fileOut) {
		std::cout << "Cannot open file to write! ( " << fileName << " )" << std::endl;
		return false;
	}

	fileOut.write((char*)blocks, sizeof(int16_t) * CHUNK_VOLUME);

	fileOut.close();
	return true;
}

void Chunk::CreateRenderData()
{
	renderData.vertices.clear();
	renderData.indices.clear();

	glm::vec3 cubeVerts[8];
	cubeVerts[0] = glm::vec3(0.0f, 1.0f, 1.0f); // ful
	cubeVerts[1] = glm::vec3(0.0f, 0.0f, 1.0f); // fll
	cubeVerts[2] = glm::vec3(1.0f, 0.0f, 1.0f); // flr
	cubeVerts[3] = glm::vec3(1.0f, 1.0f, 1.0f); // ful

	cubeVerts[4] = glm::vec3(0.0f, 1.0f, 0.0f); // bul
	cubeVerts[5] = glm::vec3(0.0f, 0.0f, 0.0f); // bll
	cubeVerts[6] = glm::vec3(1.0f, 0.0f, 0.0f); // blr
	cubeVerts[7] = glm::vec3(1.0f, 1.0f, 0.0f); // bul

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_DEPTH; z++) {
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				BlockType type = (BlockType)blocks[GetXYZIndex(x, y, z)];
				if (type == BlockType::AIR) continue;

				glm::vec3 offset(x, y, z);
				UVVertex faceVerts[4];
				int indicesOffset = renderData.vertices.size();

				// Front Face
				if (z == 15 || (z < 15 && blocks[GetXYZIndex(x, y, z + 1)] == 0)) {
					SetVertexData(faceVerts[0], cubeVerts[0] + offset, BlockFace::FRONT, type, VertexPos::LU);
					SetVertexData(faceVerts[1], cubeVerts[1] + offset, BlockFace::FRONT, type, VertexPos::LD);
					SetVertexData(faceVerts[2], cubeVerts[2] + offset, BlockFace::FRONT, type, VertexPos::RD);
					SetVertexData(faceVerts[3], cubeVerts[3] + offset, BlockFace::FRONT, type, VertexPos::RU);

					PushVertices(renderData.vertices, faceVerts);
					PushIndices(renderData.indices, indicesOffset);
					indicesOffset += 4;
				}
				// Back Face
				if (z == 0 || (z > 0 && blocks[GetXYZIndex(x, y, z - 1)] == 0)) {
					SetVertexData(faceVerts[0], cubeVerts[7] + offset, BlockFace::BACK, type, VertexPos::LU);
					SetVertexData(faceVerts[1], cubeVerts[6] + offset, BlockFace::BACK, type, VertexPos::LD);
					SetVertexData(faceVerts[2], cubeVerts[5] + offset, BlockFace::BACK, type, VertexPos::RD);
					SetVertexData(faceVerts[3], cubeVerts[4] + offset, BlockFace::BACK, type, VertexPos::RU);

					PushVertices(renderData.vertices, faceVerts);
					PushIndices(renderData.indices, indicesOffset);
					indicesOffset += 4;
				}

				// Left Face
				if (x == 0 || (x > 0 && blocks[GetXYZIndex(x - 1, y, z)] == 0)) {
					SetVertexData(faceVerts[0], cubeVerts[4] + offset, BlockFace::LEFT, type, VertexPos::LU);
					SetVertexData(faceVerts[1], cubeVerts[5] + offset, BlockFace::LEFT, type, VertexPos::LD);
					SetVertexData(faceVerts[2], cubeVerts[1] + offset, BlockFace::LEFT, type, VertexPos::RD);
					SetVertexData(faceVerts[3], cubeVerts[0] + offset, BlockFace::LEFT, type, VertexPos::RU);

					PushVertices(renderData.vertices, faceVerts);
					PushIndices(renderData.indices, indicesOffset);
					indicesOffset += 4;
				}
				// Right Face
				if (x == 15 || (x < 15 && blocks[GetXYZIndex(x + 1, y, z)] == 0)) {
					SetVertexData(faceVerts[0], cubeVerts[3] + offset, BlockFace::RIGHT, type, VertexPos::LU);
					SetVertexData(faceVerts[1], cubeVerts[2] + offset, BlockFace::RIGHT, type, VertexPos::LD);
					SetVertexData(faceVerts[2], cubeVerts[6] + offset, BlockFace::RIGHT, type, VertexPos::RD);
					SetVertexData(faceVerts[3], cubeVerts[7] + offset, BlockFace::RIGHT, type, VertexPos::RU);

					PushVertices(renderData.vertices, faceVerts);
					PushIndices(renderData.indices, indicesOffset);
					indicesOffset += 4;
				}

				// Up Face
				if (y == 255 || (x < 255 && blocks[GetXYZIndex(x, y + 1, z)] == 0)) {
					SetVertexData(faceVerts[0], cubeVerts[4] + offset, BlockFace::TOP, type, VertexPos::LU);
					SetVertexData(faceVerts[1], cubeVerts[0] + offset, BlockFace::TOP, type, VertexPos::LD);
					SetVertexData(faceVerts[2], cubeVerts[3] + offset, BlockFace::TOP, type, VertexPos::RD);
					SetVertexData(faceVerts[3], cubeVerts[7] + offset, BlockFace::TOP, type, VertexPos::RU);

					PushVertices(renderData.vertices, faceVerts);
					PushIndices(renderData.indices, indicesOffset);
					indicesOffset += 4;
				}

				// Bottom Face
				if (y == 0 || (x > 0 && blocks[GetXYZIndex(x, y - 1, z)] == 0)) {
					SetVertexData(faceVerts[0], cubeVerts[1] + offset, BlockFace::BOTTOM, type, VertexPos::LU);
					SetVertexData(faceVerts[1], cubeVerts[5] + offset, BlockFace::BOTTOM, type, VertexPos::LD);
					SetVertexData(faceVerts[2], cubeVerts[6] + offset, BlockFace::BOTTOM, type, VertexPos::RD);
					SetVertexData(faceVerts[3], cubeVerts[2] + offset, BlockFace::BOTTOM, type, VertexPos::RU);

					PushVertices(renderData.vertices, faceVerts);
					PushIndices(renderData.indices, indicesOffset);
				}
			}
		}
	}
}

void Chunk::ReleaseGPU()
{
	renderData.vb.Release();
	renderData.va.Release();
	renderData.ib.Release();
}

void Chunk::LoadCPU(int posX, int posZ, int seed)
{
	SetChunkData(posX, posZ, seed);
	CreateRenderData();
}

void Chunk::LoadGPU()
{
	VertexBufferLayout layout;
	layout.Push<uint32_t>(1);
	layout.Push<uint32_t>(1);

	renderData.vb.SetData(renderData.vertices.data(), renderData.vertices.size() * sizeof(UVVertex));
	renderData.va.AddBuffer(renderData.vb, layout);
	renderData.ib.SetData(renderData.indices.data(), renderData.indices.size());
}