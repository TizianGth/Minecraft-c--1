#pragma once

#include "vector.h"
#include "Model.h"
#include "Blockytpe.h"

const int ChunkSizeX = 1;
const int ChunkSizeY = 1;
const int ChunkSizeZ = 1;

class Chunk {
public:
	Chunk(std::vector<Blocktype> &blocks);
	~Chunk();

	void Set();
	Mesh m_Mesh;
	Model m_Model;
private:
	Blocktype m_Blocks[ChunkSizeX][ChunkSizeY][ChunkSizeZ];
	std::vector<float> ConvertPositionToVertices(vector3Int position);
	std::vector<int> ConvertPositionToIndex(int blockCount);
};