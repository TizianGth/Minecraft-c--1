#pragma once

#include "Chunk.h"
#include <iostream>


class ChunkManager {
public:
	~ChunkManager();

	static ChunkManager& Get();
	void GenerateChunks();
	const int GetDimensions();
	std::vector<std::vector<Chunk*>>& GetChunksPointer();

private:
	ChunkManager();
	static ChunkManager s_Instance;
	int ChunkRenderDistance = 5; // eg 1 active chunk (player in) + 5 in each direction = 11x11 Chunks
	std::vector<std::vector<Chunk*>> m_Chunks;
};