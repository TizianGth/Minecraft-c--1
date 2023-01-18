#include "ThreadManager.h"
#include <iostream>
#include "ChunkManager.h"
#include <chrono>


ThreadManager ThreadManager::s_Instance;

ThreadManager::~ThreadManager()
{

}

std::vector<std::shared_ptr<Chunk>>* ThreadManager::GetChunksPointer()
{
	return &m_Chunks;
}

void ThreadManager::UpdateOnMainThread()
{
	GenerateChunksOnMainThread();
}

void ThreadManager::GenerateChunks(int ChunkChangeX, int ChunkChangeZ, Vector2::Int GlobalOffset)
{
	std::async(std::launch::async, &ThreadManager::GenerateChunksMultiThread, *this, ChunkChangeX, ChunkChangeZ, GlobalOffset);
}

void ThreadManager::GenerateChunksMultiThread(int ChunkChangeX, int ChunkChangeZ, Vector2::Int GlobalOffset)
{
	int Dimensions = ChunkManager::Get().GetDimensions();
	for (int edge = 0; edge < Dimensions; edge++) {
		Vector2::Int chunk;
		if (ChunkChangeX > 0) {
			chunk = Vector2::Int(Dimensions - 1, edge);
			GenerateChunk(chunk, chunk + GlobalOffset);
		}
		else if (ChunkChangeX < 0) {
			chunk = Vector2::Int(0, edge);
			GenerateChunk(chunk, chunk + GlobalOffset);
		} else if (ChunkChangeZ > 0) {
			chunk = Vector2::Int(edge, Dimensions - 1);
			GenerateChunk(chunk, chunk + GlobalOffset);
		}
		else if (ChunkChangeZ < 0) {
			chunk = Vector2::Int(edge, 0);
			GenerateChunk(chunk, chunk + GlobalOffset);
		}
	}
}


void ThreadManager::GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkGlobalPosition)
{
	std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>();
	ChunkManager::Get().GetChunksPointer()->at(chunkPosition.x).at(chunkPosition.y) = newChunk;
	std::future<bool> t = std::async(std::launch::async, &Chunk::GenerateChunk, newChunk, chunkPosition, chunkGlobalPosition);
	m_Threads.push_back(new Thread(m_ThreadsRunning, t));
	m_Chunks.push_back(newChunk);

	m_ThreadsRunning++;
}

void ThreadManager::ResetChunks()
{
	nextChunk = nullptr;

	for (int i = 0; i < m_Chunks.size(); i++) {
		m_Chunks[i] = nullptr;
	}
}

void ThreadManager::GenerateChunksOnMainThread()
{
	if (m_Chunks.size() == 0) {
		return;
	}
	nextChunk = ReturnNextChunkToGenerate();

	if (nextChunk != nullptr) {
		nextChunk->Bind();
		nextChunk->m_Render = true;
		m_Chunks.pop_back();
		delete m_Threads.back();
		m_Threads.pop_back();

		m_ThreadsRunning--;
	}
}

std::shared_ptr<Chunk> ThreadManager::ReturnNextChunkToGenerate()
{

	// TODO: Very fragile should use hash map insted, but idc atm :)
	if (m_ThreadsRunning > 0 && m_Threads.back()->m_Finished.get()) {
		return m_Chunks.back();
	}
	return nullptr;
}




