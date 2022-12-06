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

	Mesh m_Mesh;
	Model m_Model;
private:
	//Blocktype m_Blocks[ChunkSizeX][ChunkSizeY][ChunkSizeZ];
	unsigned char m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
	glm::vec3 m_localBlockPositions[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
	std::vector<int> ConvertPositionToVertices(glm::vec3& position, int materialID);
	std::vector<int> ConvertPositionToIndex(int blockCount, Faces faces);
	Faces GetNeighbouringBlocks(glm::vec3& position);
	int ConvertVector3ToIndex(glm::vec3& position);

};