#include "ChunkGenerator.h"
#include <iostream>
#include <cstddef>
#include <iostream>
#include <iterator>
ChunkGenerator::ChunkGenerator(int size)
{
	m_Size = size;
}

ChunkGenerator::~ChunkGenerator()
{
	m_ShouldUpdate = false;
	m_UpdateThread.join();
}

void ChunkGenerator::Update(Camera& cam)
{
	m_UpdateThread = std::thread(&ChunkGenerator::UpdateChunks, this, std::ref(cam));
}

void ChunkGenerator::BindChunks()
{
	m_Mutex.lock();
	int i = 0;
	for (auto& it : m_AllChunks) {
		if (i >= m_ChunksBindPerFrame)
			break;

		Chunk* chunk = it.second;
		if (chunk == nullptr || chunk->m_Bound || !chunk->m_Generated)
			continue;
		Model* m = new Model();
		chunk->Bind(m);
		i++;
	}
	m_Mutex.unlock();

}

void ChunkGenerator::DeleteChunks()
{
	m_Mutex.lock();
	int size = m_DeleteQueue.size();

	for (int i = 0; i < size && i < m_ChunksDeletePerFrame; i++) {
		Chunk* chunk = m_DeleteQueue.front();
		m_DeleteQueue.pop();
		delete chunk;
	}
	m_Mutex.unlock();
}

void ChunkGenerator::GenerateChunk(Chunk* chunk, glm::ivec2 chunkPos)
{
	m_Mutex.lock();

	std::thread t([&]() {
		chunk->Generate();
		generate = chunk;
		m_StartUp = true;
	});

	m_Mutex.unlock();
}

Settings::BlockTypes ChunkGenerator::GetBlock(const glm::vec3& position)
{
	glm::ivec3 blockPos;
	glm::ivec2 chunkPosition;
	WorldToChunkSpace(position, blockPos, chunkPosition);
	return GetBlockChunk(blockPos, chunkPosition);
}

Settings::BlockTypes ChunkGenerator::GetBlockChunk(const glm::ivec3& blockPosition, const glm::ivec2& chunkPosition)
{
	if (m_AllChunks.contains(chunkPosition)) {
		Settings::BlockTypes block = m_AllChunks[chunkPosition]->Getblock(blockPosition.x, blockPosition.y, blockPosition.z);
		return block;
	}
	return Settings::BlockTypes::INVALID;
}

void ChunkGenerator::WorldToChunkSpace(const glm::vec3& position, glm::ivec3& blockPosition, glm::ivec2& chunkPosition)
{
	float _x = std::floor(position.x) / (float)CHUNK_SIZE;
	float _z = std::floor(position.z) / (float)CHUNK_SIZE;

	chunkPosition = glm::ivec2(std::floor(_x), std::floor(_z));

	int blockPosX = position.x - (chunkPosition.x * CHUNK_SIZE);
	int blockPosZ = position.z - (chunkPosition.y * CHUNK_SIZE);

	if (blockPosX < 0) {
		blockPosX += 16;
	}
	if (blockPosZ < 0) {
		blockPosZ += 16;
	}

	blockPosition = glm::ivec3(blockPosX, (int)position.y, blockPosZ);
}



void ChunkGenerator::UpdateChunks(Camera& cam)
{
	while (m_ShouldUpdate) {
		glm::ivec3 playerPos = glm::ivec3(std::round(cam.m_Position.x), cam.m_Position.y, std::round(cam.m_Position.z));
		glm::vec2 chunkPos = glm::vec2(std::floor((float)playerPos.x / CHUNK_SIZE), std::floor((float)playerPos.z / CHUNK_SIZE));

		if (!m_StartUp && chunkPos.x == (m_LastPos.x) && chunkPos.y == (m_LastPos.y))
			continue;

		m_DeleteChunks = m_AllChunks;

		int xStart =	chunkPos.x - m_Size;
		int xEnd =		chunkPos.x + m_Size;

		int zStart =	chunkPos.y - m_Size;
		int zEnd =		chunkPos.y + m_Size;

		for (int x = xStart - 1; x < xEnd + 1; x++) {
		for (int z = zStart - 1; z < zEnd + 1; z++) {
			if (!m_ShouldUpdate) return;
				glm::ivec2 pos = glm::ivec2(x, z);
				if (x < xEnd && x >= xStart && z < zEnd && z >= zStart) {
					if (!m_AllChunks.contains(pos)) {
						Chunk* chunk = new Chunk(x, z);
						m_AllChunks[pos] = chunk;
						m_GenChunks[pos] = chunk;
					}
					else if (m_AllChunks[pos]->m_Hide) {
						m_GenChunks[pos] = m_AllChunks[pos];
					}
					m_AllChunks[pos]->m_Hide = false;
				}
				else {
					if (!m_AllChunks.contains(pos)) {
						Chunk* chunk = new Chunk(x, z);
						m_AllChunks[pos] = chunk;
					}
					m_AllChunks[pos]->m_Hide = true;
				}
				m_DeleteChunks.erase(pos);
			}
		} 
		

		for (auto& it : m_DeleteChunks) {
			if (!m_ShouldUpdate) return;
			Chunk* chunk = it.second;
			m_AllChunks.erase(it.first);
			m_GenChunks.erase(it.first);
			m_DeleteQueue.push(chunk);
		}


		if (generate != nullptr) {
			generate->Generate();
			generate = nullptr;
		}

		for (auto& it : m_GenChunks) {
			if (!m_ShouldUpdate) return;
			Chunk* chunk = it.second;

			chunk->m_NeighbourChunks[glm::ivec2(-1, 0)] = m_AllChunks[it.first + glm::ivec2(-1, 0)];
			chunk->m_NeighbourChunks[glm::ivec2(1, 0)] = m_AllChunks[it.first + glm::ivec2(1, 0)];
			chunk->m_NeighbourChunks[glm::ivec2(0, 1)] = m_AllChunks[it.first + glm::ivec2(0, 1)];
			chunk->m_NeighbourChunks[glm::ivec2(0, -1)] = m_AllChunks[it.first + glm::ivec2(0, -1)];

			if (chunk != nullptr)
				chunk->Generate();
		}

		m_GenChunks.clear();
		m_LastPos = chunkPos;
		m_StartUp = false;

	}

}








