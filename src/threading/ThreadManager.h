#pragma once
#include <vector>
#include "Chunk.h"
#include "Thread.h"
#include <unordered_map>
#include "vector.h"

class ThreadManager {
public:
	~ThreadManager();

	void GenerateChunk(Vector2::Int chunkPosition, Vector2::Int globalChunkPosition);
	void CheckForUpdates();

	unsigned int m_ThreadsRunning;
	std::vector<Thread> m_Threads;

	inline static ThreadManager& Get() { return s_Instance; };
private:

	std::unordered_map<int, Chunk*> m_Chunks;
	static ThreadManager s_Instance;
}; 