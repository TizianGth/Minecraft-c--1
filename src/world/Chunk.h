#pragma once

#include "Model.h"
#include "Blockytpe.h"
#include "Faces.h"
#include "PerlinNoise.h"
#include "vector.h"
#include <array>
#include "BoxCollider.h"

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

	bool GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition);

	const int GetBlock(glm::vec3 position);

	std::array<unsigned char, CHUNK_SIZE* CHUNK_SIZE* CHUNK_HEIGHT>& GetBlocks();
public:
	Model m_Model;
	Mesh* m_Mesh;
	Vector2::Int m_ChunkPosition;
	Vector2::Int m_ChunkWorldPosition;

	bool m_ActiveChunk = false;
	bool m_Render = false;
private:
	std::vector<float> ConvertPositionToVertices(glm::vec3 position, int materialID);
	std::vector<unsigned char> GetVertexMaterialID(unsigned char materialID);
	std::vector<unsigned int> ConvertPositionToIndex(int blockCount, Faces faces);
	Faces GetNeighbouringBlocks(glm::vec3 position);
	const int GetIndex(glm::vec3 position);

private:
	std::array<unsigned char, CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT> m_Blocks;
	BoxCollider m_Colliders[CHUNK_SIZE][CHUNK_SIZE];
	// TODO: maybe make this some kind of pointer and delete later, so it dosnt always store this data
	unsigned char m_BlocksEdge[4][CHUNK_SIZE][CHUNK_HEIGHT]; // 0 = top (z + 1), 1 = right (x + 1), 2 = bottom (z=-1), 3 = left (x=-1)
	Vector3::UnsignedChar m_localBlockPositions[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
};