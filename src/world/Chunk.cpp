#include <iostream>
#include <stdint.h>
#include <chrono>
#include "ChunkManager.h"
#include <algorithm>

//TODO: CHUNK AND WORLD POSITION CONVERSION
Chunk::Chunk()
{

}
Chunk::~Chunk()
{

}

std::vector<int> Chunk::ConvertPositionToVertices(glm::vec3 position, int materialID) {
	int x = position.x;
	int y = position.y;
	int z = position.z;
	std::vector<int> result = {
		 x		,  y		, (z + 1)	, -1, -1,  1, materialID,
		(x + 1)	,  y		, (z + 1)	,  1, -1,  1, materialID,
		(x + 1)	, (y + 1)	, (z + 1)	,  1,  1,  1, materialID,
		 x		, (y + 1)	, (z + 1)	, -1,  1,  1, materialID,

		x		,  y		,  z		, -1, -1, -1, materialID,
	   (x + 1)	,  y		,  z		,  1, -1, -1, materialID,
	   (x + 1)	, (y + 1)	,  z		,  1,  1, -1, materialID,
		x		, (y + 1)	,  z		, -1,  1, -1, materialID
	};

	return result;
}

std::vector<int> Chunk::ConvertPositionToIndex(int blockCount, Faces faces)
{
	std::vector<int> frontIndices = {
		// Front
		 0 + (8 * blockCount),1 + (8 * blockCount),2 + (8 * blockCount),
		 2 + (8 * blockCount),3 + (8 * blockCount),0 + (8 * blockCount)
	};
	std::vector<int> backIndices = {
		// Back
		4 + (8 * blockCount),5 + (8 * blockCount),6 + (8 * blockCount),
		6 + (8 * blockCount),7 + (8 * blockCount),4 + (8 * blockCount),
	};
	std::vector<int> rightIndices = {
		// Right
		1 + (8 * blockCount), 5 + (8 * blockCount), 6 + (8 * blockCount),
		6 + (8 * blockCount), 2 + (8 * blockCount), 1 + (8 * blockCount)
	};
	std::vector<int> leftIndices = {
		// Left
		0 + (8 * blockCount), 4 + (8 * blockCount), 7 + (8 * blockCount),
		7 + (8 * blockCount), 3 + (8 * blockCount), 0 + (8 * blockCount)
	};
	std::vector<int> topIndices = {
		// Top
		2 + (8 * blockCount), 3 + (8 * blockCount), 7 + (8 * blockCount),
		7 + (8 * blockCount), 2 + (8 * blockCount), 6 + (8 * blockCount)
	};
	std::vector<int> bottomIndices = {
		// Bottom
		1 + (8 * blockCount), 5 + (8 * blockCount), 4 + (8 * blockCount),
		4 + (8 * blockCount), 1 + (8 * blockCount), 0 + (8 * blockCount)
	};


	std::vector<int> result;
	// result.reserve(faces.Count() * 6);

	if (faces.faces[faces.front]) {
		result.insert(result.end(), frontIndices.begin(), frontIndices.end());
	}
	if (faces.faces[faces.back]) {
		result.insert(result.end(), backIndices.begin(), backIndices.end());
	}
	if (faces.faces[faces.right]) {
		result.insert(result.end(), rightIndices.begin(), rightIndices.end());
	}
	if (faces.faces[faces.left]) {
		result.insert(result.end(), leftIndices.begin(), leftIndices.end());
	}
	if (faces.faces[faces.top]) {
		result.insert(result.end(), topIndices.begin(), topIndices.end());
	}
	if (faces.faces[faces.bottom]) {
		result.insert(result.end(), bottomIndices.begin(), bottomIndices.end());
	}

	return result;
}

void Chunk::FillUpTest()
{
	// leave 1 block on each side otherwise, faces on the outside wont be rendered because they touch the other chunk
	// TODO: debug why if face touches other chunk sometimes itll still get rendered
	glm::vec3 position = glm::vec3(0, 0, 0);

	 siv::PerlinNoise::seed_type seed = 223456u;

	 siv::PerlinNoise perlin{ seed };

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			int noise = (int)(perlin.octave2D_01((x + (m_ChunkPosition.x * CHUNK_SIZE)) * 0.0625, (z + (m_ChunkPosition.y * CHUNK_SIZE)) * 0.0625, 3) * 10) + 1;
			//std::cout << noise << std::endl;
			for (int y = 0; y < noise; y++) {
				position = glm::vec3(x, y, z);
				m_blocks[GetIndex(position)] = 1;

				position = glm::vec3(x, y, z);
				m_blocks[GetIndex(position)] = 2;
			}
		}
	}
}

void Chunk::SetChunkPosition(glm::vec2 chunkPosition)
{
	m_ChunkPosition = chunkPosition;
}


/// <summary>
/// Fill whole chunk with air blocks (blockId = 0) or any other block
/// </summary>
void Chunk::Generate()
{
	// 1. fill first row  (x,0,0) and keep track when finished then currentZLevel += 1, then when both finished currentYLevel += 1;
	int progressToNextZ = 0;    // same as x
	int progressToNextY = 0;
	int currentZLevel = 0;      // same as z
	int currentYLevel = 0;      // sane as y

	// this is the data that will be saved in the end
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
		m_blocks[i] = 0;

		Vector3::UnsignedChar position(progressToNextZ, currentYLevel, currentZLevel);
		m_localBlockPositions[i] = position;

		progressToNextZ++;
		progressToNextY++;

		if (progressToNextZ == CHUNK_SIZE) {
			progressToNextZ = 0;
			currentZLevel++;
		}

		if (progressToNextY == CHUNK_SIZE * CHUNK_SIZE) {
			progressToNextY = 0;
			progressToNextZ = 0;
			currentZLevel = 0;
			currentYLevel++;
		}

	}
}



void Chunk::GenerateMeshes()
{
	Mesh mesh;

	// Accounts for Air blocks, because they shouldnt be counted to index
	int minus = 0;

	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
		if (m_blocks[i] == 0) {
			minus++;
			continue;
		}
		glm::vec3 position = glm::vec3(m_localBlockPositions[i].x, m_localBlockPositions[i].y, m_localBlockPositions[i].z);
		int materialID = m_blocks[i];
		Faces faces = GetNeighbouringBlocks(position);

		if (faces.Count() == 6) continue;

		std::vector<int> newVertices = ConvertPositionToVertices(position, materialID);
		int newVerticesLength = newVertices.size();

		mesh.vertices.reserve(newVerticesLength);

		for (int v = 0; v < newVerticesLength; v++) {
			mesh.vertices.emplace_back(newVertices[v]);
		}

		std::vector<int> newIndices = ConvertPositionToIndex(i - minus, faces);
		int newIndicesLength = newIndices.size();

		mesh.indices.reserve(newIndicesLength);

		for (int v = 0; v < newIndicesLength; v++) {
			mesh.indices.emplace_back(newIndices[v]);
		}
	}

	m_Model.Set(mesh);
	m_Model.addVB();
	m_Model.addIB();
	m_Model.addVA();

}


/// <summary>
/// creates "seams" on chunks, to remove them youd need a global block buffer or check sorounding chunks. 
/// Can be important when y is getting greater. Not worth it atm
/// </summary>

Faces Chunk::GetNeighbouringBlocks(glm::vec3 position)
{
	Faces localFaces;

	glm::vec3 offsetPosition = position + glm::vec3(1, 0, 0);
	int block = GetBlockFromOtherChunk(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.right] = true;
	}
	offsetPosition = position + glm::vec3(-1, 0, 0);
	block = GetBlockFromOtherChunk(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.left] = true;
	}

	offsetPosition = position + glm::vec3(0, 1, 0);
	block = GetBlockFromOtherChunk(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.top] = true;
	}

	offsetPosition = position + glm::vec3(0, -1, 0);
	block = GetBlockFromOtherChunk(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.bottom] = true;
	}

	offsetPosition = position + glm::vec3(0, 0, -1);
	block = GetBlockFromOtherChunk(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.back] = true;
	}

	offsetPosition = position + glm::vec3(0, 0, 1);
	block = GetBlockFromOtherChunk(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.front] = true;
	}

	return localFaces;
}

const int Chunk::GetBlockFromOtherChunk(glm::vec3 position)
{
	if (position.y >= CHUNK_HEIGHT || position.y < 0) return 0; // return "air block" when at max y build limit 
	int desiredChunkX = m_ChunkPosition.x;
	int desiredChunkZ = m_ChunkPosition.y;
	glm::vec3 inChunkPosition = glm::vec3(position.x, position.y, position.z);
	if (position.x >= CHUNK_SIZE) {
		desiredChunkX = m_ChunkPosition.x + 1;
		inChunkPosition.x = position.x - CHUNK_SIZE;
	}
	else if (position.x < 0) {
		desiredChunkX = m_ChunkPosition.x - 1;
		inChunkPosition.x = CHUNK_SIZE + position.x;
	}
	if (position.z >= CHUNK_SIZE) {
		desiredChunkZ = m_ChunkPosition.y + 1;
		inChunkPosition.z = position.z - CHUNK_SIZE;
	}
	else if (position.z < 0) {
		desiredChunkZ = m_ChunkPosition.y - 1;
		inChunkPosition.z = CHUNK_SIZE + position.z;;
	}

	if (desiredChunkX < 0 || desiredChunkZ < 0 || desiredChunkX >= ChunkManager::Get().GetDimensions() || desiredChunkZ >= ChunkManager::Get().GetDimensions()) {
		return -1;
	}
	if (desiredChunkX >= 0 && desiredChunkZ >= 0) {
		auto ref = ChunkManager::Get();
		int id = ref.GetChunksPointer()[desiredChunkX][desiredChunkZ]->GetBlock(inChunkPosition);
		return id;
	}
}

const int Chunk::GetIndex(glm::vec3 position)
{
	if (position.x < 0 || position.x >= CHUNK_SIZE || position.z < 0 || position.z >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT) return -1;

	return position.x + position.y * (CHUNK_HEIGHT)+position.z * (CHUNK_SIZE);
}

const int Chunk::GetBlock(glm::vec3 position)
{
	int index = GetIndex(position);
	int id = m_blocks[index];
	
	return id;
}