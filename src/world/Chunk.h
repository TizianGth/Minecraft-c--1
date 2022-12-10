#pragma once

#include "Model.h"
#include "Blockytpe.h"
#include "Faces.h"

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 256;

class Chunk {
public:
	Chunk();
	~Chunk();

	void Generate();
	void GenerateMeshes();
	void FillUpTest();
	void SetAllChunksPointer(Chunk* chunks[14][14]);
	void SetChunkPosition(glm::vec2 chunkPosition);

	Mesh m_Mesh;
	Model m_Model;
	Chunk* m_AllChunks[14][14];
	glm::vec2 m_ChunkPosition;
private:
	unsigned char m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
	glm::vec3 m_localBlockPositions[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
	std::vector<int> ConvertPositionToVertices(glm::vec3& position, int materialID);
	std::vector<int> ConvertPositionToIndex(int blockCount, Faces faces);
	Faces GetNeighbouringBlocks(glm::vec3& position);
	int GetBlockFromOtherChunk(glm::vec3& position);
	int GetIndex(glm::vec3& position);
	int GetBlock(glm::vec3& position);
};