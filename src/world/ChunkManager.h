#pragma once

#include "Chunk.h"
#include <iostream>
#include "vector.h"


class ChunkManager {
public:
	~ChunkManager();

	static ChunkManager& Get();
	void GenerateChunks();
	std::shared_ptr<Chunk> GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition);
	void UpdateChunks(Vector2::Int playerChunk);
	void ShiftChunks(int ChunkChangeX, int ChunkChangeZ);
	const int GetDimensions();
	std::vector<std::vector<std::shared_ptr<Chunk>>>* GetChunksPointer();
	Vector2::Int m_ActivLastChunk;
	std::vector<std::vector<std::shared_ptr<Chunk>>> m_Chunks;
	Vector2::Int m_GlobalChunkOffset;

	bool m_ShiftingChunks = false;

private:
	ChunkManager();
	void SetNewActiveChunks(int ChunkChangeX, int ChunkChangeZ);
	static ChunkManager s_Instance;
	int ChunkRenderDistance = 10; // eg 1 active chunk (player in) + 5 in each direction = 11x11 Chunks 24 is max after that partially out of view
	
};