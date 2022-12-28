#pragma once

#include "Model.h"
#include "Blockytpe.h"
#include "Faces.h"
#include "PerlinNoise.h"
#include "vector.h"
#include <array>

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 256;

class Chunk {
public:
	Chunk();
	~Chunk();

	void Generate();
	bool GenerateMeshes();
	void FillUpTest();
	void SetChunkPosition(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition);
	void Bind();
public:
	Model m_Model;
	Vector2::Int m_ChunkPosition;
	Vector2::Int m_ChunkWorldPosition;
	bool m_ActiveChunk = false;
private:
	std::vector<float> ConvertPositionToVertices(glm::vec3 position, int materialID);
	std::vector<unsigned char> GetVertexMaterialID(unsigned char materialID);
	std::vector<unsigned int> ConvertPositionToIndex(int blockCount, Faces faces);
	Faces GetNeighbouringBlocks(glm::vec3 position);
	const int GetIndex(glm::vec3 position);
	const int GetBlock(glm::vec3 position);

private:
	unsigned char m_Blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
	// TODO: maybe make this some kind of pointer and delete later, so it dosnt always store this data
	unsigned char m_BlocksEdge[4][CHUNK_SIZE][CHUNK_HEIGHT]; // 0 = top (z + 1), 1 = right (x + 1), 2 = bottom (z=-1), 3 = left (x=-1)
	Vector3::UnsignedChar m_localBlockPositions[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
};