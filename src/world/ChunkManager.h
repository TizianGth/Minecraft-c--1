#pragma once

#include "Chunk.h"
#include <iostream>
#include "vector.h"


class ChunkManager {
public:
	~ChunkManager();

	static ChunkManager& Get();
	void GenerateChunks();
	Chunk* GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition);
	void UpdateChunks(Vector2::Int playerChunk);
	const int GetDimensions();
	std::vector<std::vector<Chunk*>>& GetChunksPointer();
	Vector2::Int m_ActivLastChunk;
	std::vector<std::vector<Chunk*>> m_Chunks;

private:
	ChunkManager();
	void SetNewActiveChunks(int ChunkChangeX, int ChunkChangeZ);
	static ChunkManager s_Instance;
	int ChunkRenderDistance = 5; // eg 1 active chunk (player in) + 5 in each direction = 11x11 Chunks
	//std::vector<Chunk*> m_GarbageChunks;
	Vector2::Int m_GlobalChunkOffset;
};