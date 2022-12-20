#pragma once

#include "Chunk.h"
#include <iostream>
#include "vector.h"


class ChunkManager {
public:
	~ChunkManager();

	static ChunkManager& Get();
	void GenerateChunks();
	void UpdateChunks(Vector2::Int playerChunk);
	const int GetDimensions();
	std::vector<std::vector<Chunk*>>& GetChunksPointer();
	Vector2::Int m_ActivLastChunk;

private:
	ChunkManager();
	void GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition);
	static ChunkManager s_Instance;
	int ChunkRenderDistance = 5; // eg 1 active chunk (player in) + 5 in each direction = 11x11 Chunks
	std::vector<std::vector<Chunk*>> m_Chunks;
	std::vector<Chunk*> m_GarbageChunks;
	Vector2::Int m_GlobalChunkOffset;
};