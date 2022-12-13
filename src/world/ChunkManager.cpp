#include "ChunkManager.h"

ChunkManager ChunkManager::s_Instance;

ChunkManager& ChunkManager::Get()
{
	return s_Instance;
}

ChunkManager::ChunkManager()
{
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
			m_Chunks[x][z]->SetChunkPosition(glm::vec2(x, z));
			m_Chunks[x][z]->Generate();
			m_Chunks[x][z]->FillUpTest(); 
		}
	}
	for (int z = 0; z < ChunkDimensions; z++) {
		for (int x = 0; x < ChunkDimensions; x++) {
			// TODO: I shouldnt set the allChunksPointer on each Chunk (because theire always the same), instead have them stored in eg this class
			// then I could also move "GenerateMeshes" in the first for loop and wouldnt need the second for loop anymore
			//m_Chunks[x][z]->SetAllChunksPointer(&m_Chunks);
			m_Chunks[x][z]->GenerateMeshes();
		}
	}
}

const int ChunkManager::GetDimensions()
{
	return  ChunkRenderDistance * 2 + 1;
}

std::vector<std::vector<Chunk*>>& ChunkManager::GetChunksPointer()
{
	return m_Chunks;
}
