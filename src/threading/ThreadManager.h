#pragma once
#include <vector>
#include "Chunk.h"
#include "Thread.h"
#include <unordered_map>
#include "vector.h"

class ThreadManager {
public:
	~ThreadManager();
	void UpdateOnMainThread();
	void GenerateChunks(int ChunkChangeX, int ChunkChangeZ, Vector2::Int GlobalOffset);
	void GenerateChunksMultiThread(int ChunkChangeX, int ChunkChangeZ, Vector2::Int GlobalOffset);
	void GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkGlobalPosition);

	unsigned int m_ThreadsRunning;
	inline static ThreadManager& Get() { return s_Instance; };
private:
	static ThreadManager s_Instance;

	void GenerateChunksOnMainThread();

	std::shared_ptr<Chunk> ReturnNextChunkToGenerate();
	std::shared_ptr<Chunk> nextChunk = nullptr;

	std::vector<std::shared_ptr<Chunk>> m_Chunks;
	std::vector<Thread*> m_Threads;
};