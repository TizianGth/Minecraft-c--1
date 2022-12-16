#include "ChunkManager.h"

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
	if (m_Chunks[chunkPosition.x][chunkPosition.y] != nullptr) {
		// TODO: i need to delete this chunk, but keeps throwing error
		m_GarbageChunks.push_back(m_Chunks[chunkPosition.x][chunkPosition.y]);
	}
	m_Chunks[chunkPosition.x][chunkPosition.y] = new Chunk;
	m_Chunks[chunkPosition.x][chunkPosition.y]->SetChunkPosition(Vector2::Int(chunkPosition.x, chunkPosition.y), Vector2::Int(chunkWorldPosition.x, chunkWorldPosition.y));
	m_Chunks[chunkPosition.x][chunkPosition.y]->Generate();
	m_Chunks[chunkPosition.x][chunkPosition.y]->FillUpTest();

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
		// Delete chunks
		for (int z = 0; z < ChunkDimensions; z++) {
			delete m_Chunks[0][z];
			m_Chunks[0][z] = nullptr;
		}
		// Shift chunks to delete chunks place
		for (int x = 0; x < ChunkDimensions - 1; x++) {
			m_Chunks[x] = m_Chunks[x + 1];
		}
		// Generate new chunks
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(ChunkDimensions - 1, z);
			GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(ChunkDimensions - 1, z);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
		}
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
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(0, z);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
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
		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, ChunkDimensions - 1);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
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
		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, 0);
			m_Chunks[chunk.x][chunk.y]->GenerateMeshes();
		}
	}

	m_ActivLastChunk = playerChunk;

	/*int size = m_GarbageChunks.size();
	for (int i = 0; i < size; i++) {
		if (m_GarbageChunks[i] == nullptr) { continue; }
		if (size > ChunkDimensions * ChunkDimensions) {
			delete m_GarbageChunks[i];
			m_GarbageChunks[i] = nullptr;
		}
	} */
}

const int ChunkManager::GetDimensions()
{
	return (ChunkRenderDistance * 2) + 1;
}


std::vector<std::vector<Chunk*>>& ChunkManager::GetChunksPointer()
{
	return m_Chunks;
}