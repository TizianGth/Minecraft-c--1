#include "ChunkManager.h"
#include "ThreadManager.h"
#include <ppl.h>

ChunkManager ChunkManager::s_Instance;

ChunkManager& ChunkManager::Get()
{
	return s_Instance;
}

ChunkManager::ChunkManager()
{
}

void ChunkManager::GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition)
{
	m_Chunks[chunkPosition.x][chunkPosition.y] = nullptr;
	Chunk* newChunk = new Chunk;
	newChunk->SetChunkPosition(Vector2::Int(chunkPosition.x, chunkPosition.y), Vector2::Int(chunkWorldPosition.x, chunkWorldPosition.y));
	newChunk->Generate();
	newChunk->FillUpTest();
	m_Chunks[chunkPosition.x][chunkPosition.y] = newChunk;

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
			m_Chunks[x][z] = new Chunk;
			m_Chunks[x][z]->SetChunkPosition(Vector2::Int(x, z), Vector2::Int(x, z));
			m_Chunks[x][z]->Generate();
			m_Chunks[x][z]->FillUpTest();
		}
	}
	for (int z = 0; z < ChunkDimensions; z++) {
		for (int x = 0; x < ChunkDimensions; x++) {
			m_Chunks[x][z]->GenerateMeshes();
			m_Chunks[x][z]->Bind();
		}
	}
}

void ChunkManager::UpdateChunks(Vector2::Int playerChunk)
{

	//
	// TODO: Load 1 block in direction to avoid faces being empty
	//
	int ChunkChangeX = playerChunk.x - m_ActivLastChunk.x;
	int ChunkChangeZ = playerChunk.y - m_ActivLastChunk.y;
	int ChunkDimensions = GetDimensions();
	m_GlobalChunkOffset += Vector2::Int(ChunkChangeX, ChunkChangeZ);

	if (ChunkChangeX > 0) {
		// Delete chunks
		for (int z = 0; z < ChunkDimensions; z++) {
			Chunk* oldChunk = m_Chunks[0][z];
			m_Chunks[0][z] = nullptr;
			delete oldChunk;
		}
		// Shift chunks to delete chunks place
		for (int x = 0; x < ChunkDimensions - 1; x++) {
			m_Chunks[x] = m_Chunks[x + 1];
		}

		//std::cout << "Making Chunk.." << std::endl;
		// Generate new chunks+
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(ChunkDimensions - 1, z);
			GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}

		//std::cout << "Generating Mesh.." << std::endl;

		concurrency::parallel_for(int(0), ChunkDimensions, [&](int z) {
			Vector2::Int chunk(ChunkDimensions - 1, z);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			});

		for (int z = 0; z < ChunkDimensions; z++) {
			m_Chunks[ChunkDimensions - 1][z]->Bind();
		}

		//std::cout << "Finished!" << std::endl;
	}

	else if (ChunkChangeX < 0) {
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
			GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}
		concurrency::parallel_for(int(0), ChunkDimensions, [&](int z) {
			Vector2::Int chunk(0, z);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			});

		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(0, z);
			m_Chunks[chunk.x][chunk.y]->Bind();
		}
	}

	if (ChunkChangeZ > 0) {
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
			GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}
		concurrency::parallel_for(int(0), ChunkDimensions, [&](int x) {
			Vector2::Int chunk(x, ChunkDimensions - 1);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			});

		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, ChunkDimensions - 1);
			m_Chunks[chunk.x][chunk.y]->Bind();
		}
	}
	else if (ChunkChangeZ < 0) {
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
			GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}
		concurrency::parallel_for(int(0), ChunkDimensions, [&](int x) {
			Vector2::Int chunk(x, 0);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
			});

		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, 0);
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