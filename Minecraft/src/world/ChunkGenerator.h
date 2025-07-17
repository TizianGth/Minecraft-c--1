#pragma once
#include "Chunk.h"

#include <thread>
#include <unordered_map>
#include <mutex>
#include <future>
#include <queue>

#include "Utils.h"
#include "Camera.h"


class ChunkGenerator
{
public:
	ChunkGenerator(int size);
	~ChunkGenerator();

	void Update(Camera& cam);

	void BindChunks();
	void DeleteChunks();

	void GenerateChunk(Chunk* chunk, glm::ivec2 chunkPos);

	Settings::BlockTypes GetBlock(const glm::vec3& position);
	Settings::BlockTypes GetBlockChunk(const glm::ivec3& blockPosition, const glm::ivec2& chunkPosition);

	void WorldToChunkSpace(const glm::vec3& position, glm::ivec3& blockPosition, glm::ivec2& chunkPosition);

	bool m_ShouldUpdate = false;
	std::thread m_UpdateThread;
	std::thread m_DeleteThread;

	glm::ivec2 m_LastPos;

	std::unordered_map<glm::ivec2, Chunk*, hash> m_AllChunks;
	std::unordered_map<glm::ivec2, Chunk*, hash> m_GenChunks;
	std::unordered_map<glm::ivec2, Chunk*, hash> m_DeleteChunks;

	Chunk* generate = nullptr;

	std::queue<Chunk*> m_DeleteQueue;

	std::mutex m_Mutex;

	bool m_StartUp = true;
private:
	void UpdateChunks(Camera& cam);


	const unsigned int m_ChunksBindPerFrame = 10;
	const unsigned int m_ChunksDeletePerFrame = 10;

	unsigned int m_Size = 0;
};

