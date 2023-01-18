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

std::shared_ptr<Chunk> ChunkManager::GenerateChunk(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition)
{
	std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>();
	newChunk->m_ActiveChunk = false;
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
			Vector2::Int position(x, z);
			m_Chunks[x][z] = GenerateChunk(position, position);
			m_Chunks[x][z]->GenerateMeshes();
			m_Chunks[x][z]->Bind();

		}
	}
}
int ChunkChangeX = 0;
int ChunkChangeZ = 0;
void ChunkManager::UpdateChunks(Vector2::Int playerChunk)
{
	m_Finished = true;

	ChunkChangeX += playerChunk.x - m_ActivLastChunk.x;
	ChunkChangeZ += playerChunk.y - m_ActivLastChunk.y;
	int ChunkDimensions = GetDimensions();
	

	if (ChunkChangeX != 0) {
		m_GlobalChunkOffset += Vector2::Int(ChunkChangeX, 0);
	} else if (ChunkChangeZ != 0) {
		m_GlobalChunkOffset += Vector2::Int(0, ChunkChangeZ);
	}
	else {
		return;
	}

	if (ChunkChangeX > 0) {
		for (int edge = 0; edge < ChunkDimensions; edge++) {
			m_Chunks[0][edge] = nullptr;

			for (int shift = 0; shift < ChunkDimensions - 1; shift++) {
				m_Chunks[shift][edge] = m_Chunks[shift + 1][edge];
				m_Chunks[shift + 1][edge] = nullptr;

			}
		}

	}

	else if (ChunkChangeX < 0) {
		for (int edge = 0; edge < ChunkDimensions; edge++) {
			m_Chunks[ChunkDimensions - 1][edge] = nullptr;

			for (int shift = ChunkDimensions - 1; shift > 0; shift--) {
				m_Chunks[shift][edge] = m_Chunks[shift - 1][edge];
				m_Chunks[shift - 1][edge] = nullptr;

			}
		}


	} 
	else if (ChunkChangeZ > 0) {
		for (int edge = 0; edge < ChunkDimensions; edge++) {
			m_Chunks[edge][0] = nullptr;

			for (int shift = 0; shift < ChunkDimensions - 1; shift++) {
				m_Chunks[edge][shift] = m_Chunks[edge][shift + 1];
				m_Chunks[edge][shift + 1] = nullptr;

			}
		}

	}

	else if (ChunkChangeZ < 0) {
		for (int edge = 0; edge < ChunkDimensions; edge++) {
			m_Chunks[edge][ChunkDimensions - 1] = nullptr;

			for (int shift = ChunkDimensions - 1; shift > 0; shift--) {
				m_Chunks[edge][shift] = m_Chunks[edge][shift - 1];
				m_Chunks[edge][shift - 1] = nullptr;
			}
		}
	}

	ThreadManager::Get().GenerateChunksMultiThread(ChunkChangeX, ChunkChangeZ, m_GlobalChunkOffset);

	if (ChunkChangeX > 0) {
		ChunkChangeX--;
	}
	else if (ChunkChangeX < 0) {
		ChunkChangeX++ ;
	} 
	else if (ChunkChangeZ > 0) {
		ChunkChangeZ--;
	}
	else if (ChunkChangeZ < 0) {
		ChunkChangeZ++;
	}


	m_ActivLastChunk = playerChunk;
}

const int ChunkManager::GetDimensions()
{
	return (ChunkRenderDistance * 2) + 1;
}


std::vector<std::vector<std::shared_ptr<Chunk>>>* ChunkManager::GetChunksPointer()
{
	return &m_Chunks;
}