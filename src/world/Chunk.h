#pragma once

#include "vector.h"
#include "Model.h"

const int ChunkSize = 9; // x = 3; y = 3; z = 1;

class Chunk {
public:
	Chunk(vector3 positions[ChunkSize]);
	~Chunk();

	void Set();
	Model m_Models[ChunkSize];
private:
};