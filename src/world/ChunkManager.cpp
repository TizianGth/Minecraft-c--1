#include "ChunkManager.h"
#include "ThreadManager.h"
#include <ppl.h>
#include <future>

ChunkManager ChunkManager::s_Instance;

ChunkManager& ChunkManager::Get()
{
	return s_Instance;
}

ChunkManager::ChunkManager()
{
}

void ChunkManager::SetNewActiveChunks(int ChunkChangeX, int ChunkChangeZ)
{
	if (ChunkChangeX != 0 && ChunkChangeZ != 0) {
		for (int x = ChunkRenderDistance - 1; x < ChunkRenderDistance + 1; x++) {
			for (int z = ChunkRenderDistance - 1; z < ChunkRenderDistance + 1; z++) {
				m_Chunks[x][z]->m_ActiveChunk = false;
			}
		}
	}
	for (int x = ChunkRenderDistance - 1 + ChunkChangeX; x < ChunkRenderDistance + 1 + ChunkChangeX; x++) {
		for (int z = ChunkRenderDistance - 1 + ChunkChangeZ; z < ChunkRenderDistance + 1 + +ChunkChangeZ; z++) {
			m_Chunks[x][z]->m_ActiveChunk = true;
		}
	}
}

Chunk* ChunkManager::GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition)
{
	Chunk* newChunk = new Chunk;
	newChunk->SetChunkPosition(Vector2::Int(chunkPosition.x, chunkPosition.y), Vector2::Int(chunkWorldPosition.x, chunkWorldPosition.y));
	newChunk->Generate();
	newChunk->FillUpTest();

	return newChunk;

}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::GenerateChunks()
{
	int ChunkDimensions = GetDimensions();
	m_Chunks.resize(ChunkDimensions);
	for (int x = 0; x < ChunkDimensions; x++) {
		m_Chunks[x].resize(ChunkDimensions);
		for (int z = 0; z < ChunkDimensions; z++) {
			m_Chunks[x][z] = GenerateChunk(Vector2::Int(x, z), Vector2::Int(x, z));

		}
	}
	for (int x = 0; x < ChunkDimensions; x++) {
		for (int z = 0; z < ChunkDimensions; z++) {
			SetNewActiveChunks(0, 0);
			m_Chunks[x][z]->GenerateMeshes();
			m_Chunks[x][z]->Bind();
		}
	}
}

void ChunkManager::UpdateChunks(Vector2::Int playerChunk)
{
	int ChunkChangeX = playerChunk.x - m_ActivLastChunk.x;
	int ChunkChangeZ = playerChunk.y - m_ActivLastChunk.y;
	int ChunkDimensions = GetDimensions();
	m_GlobalChunkOffset += Vector2::Int(ChunkChangeX, ChunkChangeZ);


	if (ChunkChangeX > 0) {
		SetNewActiveChunks(ChunkChangeX, ChunkChangeZ);
		// Delete chunks
		for (int z = 0; z < ChunkDimensions; z++) {
			Chunk* oldChunk = m_Chunks[0][z];
			m_Chunks[0][z] = nullptr;
			delete oldChunk;
		}
		// Shift chunks to delete chunks place
		std::cout << "1" << std::endl;
		for (int x = 0; x < ChunkDimensions - 1; x++) {
			m_Chunks[x] = m_Chunks[x + 1];
		}

		//std::cout << "Generating Mesh.." << std::endl;
		std::cout << "3" << std::endl;
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(ChunkDimensions - 1, z);
			ThreadManager::Get().GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
			//m_Chunks[chunk.x][chunk.y] = GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
			//m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			//m_Chunks[chunk.x][chunk.y]->Bind();
		}
	}

	else if (ChunkChangeX < 0) {
		SetNewActiveChunks(ChunkChangeX, ChunkChangeZ);
		// Delete chunks
		for (int z = 0; z < ChunkDimensions; z++) {
			delete m_Chunks[ChunkDimensions - 1][z];
			m_Chunks[ChunkDimensions - 1][z] = nullptr;
		}
		// Shift chunks to delete chunks place
		for (int x = ChunkDimensions - 1; x > 0; x--) {
			m_Chunks[x] = m_Chunks[x - 1];
		}
		// Generate new chunks

		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(0, z);
			m_Chunks[chunk.x][chunk.y] = GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			m_Chunks[chunk.x][chunk.y]->Bind();
		}
	}

	if (ChunkChangeZ > 0) {
		SetNewActiveChunks(ChunkChangeX, ChunkChangeZ);
		// Delete chunks
		for (int x = 0; x < ChunkDimensions; x++) {
			delete m_Chunks[x][0];
			m_Chunks[x][0] = nullptr;
		}
		// Shift chunks to delete chunks place
		for (int z = 0; z < ChunkDimensions - 1; z++) {
			for (int x = 0; x < ChunkDimensions; x++) {
				m_Chunks[x][z] = m_Chunks[x][z + 1];
			}
		}
		// Generate new chunks
		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, ChunkDimensions - 1);
			m_Chunks[chunk.x][chunk.y] = GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			m_Chunks[chunk.x][chunk.y]->Bind();
		}
	}
	else if (ChunkChangeZ < 0) {
		SetNewActiveChunks(ChunkChangeX, ChunkChangeZ);
		// Delete chunks
		for (int x = 0; x < ChunkDimensions; x++) {
			delete m_Chunks[x][ChunkDimensions - 1];
			m_Chunks[x][ChunkDimensions - 1] = nullptr;
		}
		// Shift chunks to delete chunks place
		for (int z = ChunkDimensions - 1; z > 0; z--) {
			for (int x = 0; x < ChunkDimensions; x++) {
				m_Chunks[x][z] = m_Chunks[x][z - 1];
			}
		}
		// Generate new chunks
		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, 0);
			m_Chunks[chunk.x][chunk.y] = GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			m_Chunks[chunk.x][chunk.y]->Bind();
		}

	}

	m_ActivLastChunk = playerChunk;
}

const int ChunkManager::GetDimensions()
{
	return (ChunkRenderDistance * 2) + 1;
}


std::vector<std::vector<Chunk*>>& ChunkManager::GetChunksPointer()
{
	return m_Chunks;
}