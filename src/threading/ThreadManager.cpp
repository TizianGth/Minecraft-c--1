#include "ThreadManager.h"
#include <iostream>
#include "ChunkManager.h"

ThreadManager ThreadManager::s_Instance;

ThreadManager::~ThreadManager()
{
}

void ThreadManager::GenerateChunk(Vector2::Int chunkPosition, Vector2::Int globalChunkPosition)
{
	ChunkManager& c = ChunkManager::Get();
	Chunk* chunk = c.GenerateChunk(chunkPosition, globalChunkPosition);

	std::future<bool> t = std::async(std::launch::async, &Chunk::GenerateMeshes, chunk);
	Thread thread(m_ThreadsRunning, t);
	m_Threads.push_back(thread);
	m_Chunks[m_ThreadsRunning] = chunk;
	m_ThreadsRunning++;
}

void ThreadManager::CheckForUpdates()
{
	ChunkManager& c = ChunkManager::Get();

	int startSize = m_ThreadsRunning-1;
	int endSize = startSize+1;
	for (int i = 0; i < m_ThreadsRunning; i++) {
		if (m_Threads[i].m_Finished.get()) {
			if (m_Chunks[i] != nullptr) {
				m_Chunks[i]->Bind();
				c.m_Chunks[m_Chunks[i]->m_ChunkPosition.x][m_Chunks[i]->m_ChunkPosition.y] = m_Chunks[i];
			}
			endSize--;
		}
	}

	for (int i = endSize; i < startSize; i++) {
		m_Chunks.erase(i);
		m_Threads.pop_back();
		m_ThreadsRunning--;
	}

}





