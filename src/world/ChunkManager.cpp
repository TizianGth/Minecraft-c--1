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
	ChunkChangeX += playerChunk.x - m_ActivLastChunk.x;
	ChunkChangeZ += playerChunk.y - m_ActivLastChunk.y;
	ChunkChangeZ = 0;
	int ChunkDimensions = GetDimensions();
	

	if (ChunkChangeX != 0) {
		m_GlobalChunkOffset += Vector2::Int(ChunkChangeX, 0);
	} else if (ChunkChangeZ != 0) {
		m_GlobalChunkOffset += Vector2::Int(0, ChunkChangeZ);
	}
	else {
		return;
	}

	auto t1 = std::chrono::high_resolution_clock::now();

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
	//std::thread t(&bar::foo, bar());
	//std::thread thread(&ThreadManager::GenerateChunks, &ThreadManager::Get(), ChunkChangeX, ChunkChangeZ, m_GlobalChunkOffset);
	//thread.detach();
	//std::async(std::launch::async, &ThreadManager::GenerateChunks, &ThreadManager::Get(), ChunkChangeX, ChunkChangeZ, m_GlobalChunkOffset);
	ThreadManager::Get().GenerateChunksMultiThread(ChunkChangeX, ChunkChangeZ, m_GlobalChunkOffset);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << duration_cast<std::chrono::milliseconds>(t2 - t1) << std::endl;



	if (ChunkChangeX > 0) {


		//std::cout << "Generating Mesh.." << std::endl;
	/*	std::cout << "3" << std::endl;
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(ChunkDimensions - 1, z);
			//ThreadManager::Get().GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		} */
		ChunkChangeX--;
	}

	else if (ChunkChangeX < 0) {

	/*	// Generate new chunks
		m_ShiftingChunks = false;
		for (int z = 0; z < ChunkDimensions; z++) {
			Vector2::Int chunk(0, z);
			ThreadManager::Get().GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		} */
		ChunkChangeX++ ;
	} 
	

	/*if (ChunkChangeZ > 0) {

		// Generate new chunks
		m_ShiftingChunks = false;
		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, ChunkDimensions - 1);
			ThreadManager::Get().GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}
	}
	else if (ChunkChangeZ < 0) {

		// Generate new chunks
		m_ShiftingChunks = false;
		for (int x = 0; x < ChunkDimensions; x++) {
			Vector2::Int chunk(x, 0);
			ThreadManager::Get().GenerateChunk(chunk, chunk + m_GlobalChunkOffset);
		}

	} */

	m_ActivLastChunk = playerChunk;
}

void ChunkManager::ShiftChunks(int ChunkChangeX, int ChunkChangeZ)
{
	if (ChunkChangeX > 0) {

	}
	else if (ChunkChangeX < 0) {

	}
	else if (ChunkChangeZ > 0) {

	}
	else if (ChunkChangeZ < 0) {

	}
}

const int ChunkManager::GetDimensions()
{
	return (ChunkRenderDistance * 2) + 1;
}


std::vector<std::vector<std::shared_ptr<Chunk>>>* ChunkManager::GetChunksPointer()
{
	return &m_Chunks;
}