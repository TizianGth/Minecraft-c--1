#include <iostream>
#include <stdint.h>
#include <chrono>
#include "ChunkManager.h"
#include <algorithm>
#include <ppl.h>

//TODO: CHUNK AND WORLD POSITION CONVERSION
Chunk::Chunk()
{

}
Chunk::~Chunk()
{

}

std::vector<char> TextCoords = {
	-1, -1,  1,
	 1, -1,  1,
	 1,  1,  1,
	-1,  1,  1,

	-1, -1, -1,
	 1, -1, -1,
	 1,  1, -1,
	-1,  1, -1
};

std::vector<float> Chunk::ConvertPositionToVertices(glm::vec3 position, int materialID) {
	int x = position.x;
	int y = position.y;
	int z = position.z;
	std::vector<float> result = {
		(float)x		, (float)y			, (float)(z + 1)	, -1, -1,  1, (float)materialID,
		(float)(x + 1)	, (float)y			, (float)(z + 1)	,  1, -1,  1, (float)materialID,
		(float)(x + 1)	, (float)(y + 1)	, (float)(z + 1)	,  1,  1,  1, (float)materialID,
		(float)x		, (float)(y + 1)	, (float)(z + 1)	, -1,  1,  1, (float)materialID,
		(float)(float)(float)(float)
		(float)x		, (float)y			, (float)z			, -1, -1, -1, (float)materialID,
		(float)(x + 1)	, (float)y			, (float)z			,  1, -1, -1, (float)materialID,
		(float)(x + 1)	, (float)(y + 1)	, (float)z			,  1,  1, -1, (float)materialID,
		(float)x		, (float)(y + 1)	, (float)z			, -1,  1, -1, (float)materialID
	};

	return result;
}

std::vector<unsigned char> Chunk::GetVertexMaterialID(unsigned char materialID)
{
	std::vector<unsigned char> result = {
		materialID,
		materialID,
		materialID,
		materialID,
				  
		materialID,
		materialID,
		materialID,
		materialID
	};

	return result;
}

std::vector<unsigned int> Chunk::ConvertPositionToIndex(int blockCount, Faces faces)
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


	std::vector<unsigned int> result;
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

	siv::PerlinNoise::seed_type seed = 123456;

	siv::PerlinNoise perlin{ seed };

	int noise = 0;
	int x;
	int z;

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			noise = (int)(perlin.octave2D_01((x + (m_ChunkWorldPosition.x * CHUNK_SIZE)) * 0.0625, (z + (m_ChunkWorldPosition.y * CHUNK_SIZE)) * 0.0625, 3) * 8) + 5;
			//std::cout << noise << std::endl;
			for (int y = 0; y < noise; y++) {

				position = glm::vec3(x, y, z);
				m_Blocks[GetIndex(position)] = 1;

				position = glm::vec3(x, y, z);
				m_Blocks[GetIndex(position)] = 2;

			}
		}
	}

	noise = 0;

	for (int edge = 0; edge < 4; edge++) {
		for (int index = 0; index < CHUNK_SIZE; index++) {

			if (edge == 0) {
				x = index;
				z = 16;
			}
			else if (edge == 1) {
				z = index;
				x = 16;
			}
			else if(edge == 2) {
				x = index;
				z = -1;
			}
			else if(edge == 3) {
				z = index;
				x = -1;
			}

			noise = (int)(perlin.octave2D_01((x + (m_ChunkWorldPosition.x * CHUNK_SIZE)) * 0.0625, (z + (m_ChunkWorldPosition.y * CHUNK_SIZE)) * 0.0625, 3) * 8) + 5;
				for (int y = 0; y < noise; y++) {
					m_BlocksEdge[edge][index][y] = 1;
				}
		}
	} 

}

void Chunk::SetChunkPosition(Vector2::Int chunkPosition, Vector2::Int chunkWorldPosition)
{
	m_ChunkPosition = chunkPosition;
	m_ChunkWorldPosition = chunkWorldPosition;

}

void Chunk::Bind()
{
	m_Model.Bind();
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
		m_Blocks[i] = 0;

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



bool Chunk::GenerateMeshes()
{

	Mesh* mesh = new Mesh; // handle deletion in Model class

	int minus = 0;

	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
		if (m_Blocks[i] == 0) {
			minus++;
			continue;
		}
		glm::vec3 position = glm::vec3(m_localBlockPositions[i].x, m_localBlockPositions[i].y, m_localBlockPositions[i].z);
		int materialID = m_Blocks[i];
		Faces faces = GetNeighbouringBlocks(position);

		if (faces.Count() == 6) { continue; }

		std::vector<float> newVertices = ConvertPositionToVertices(position, materialID);

		// TODO: store textCoords and id not in floats
		int newVerticesLength = newVertices.size();
		mesh->verticesPosition.reserve(newVerticesLength);
		for (int v = 0; v < newVerticesLength; v++) {
			mesh->verticesPosition.emplace_back(newVertices[v]);
		}

		std::vector<unsigned int> newIndices = ConvertPositionToIndex(i - minus, faces);
		int newIndicesLength = newIndices.size();

		mesh->indices.reserve(newIndicesLength);

		for (int v = 0; v < newIndicesLength; v++) {
			mesh->indices.emplace_back(newIndices[v]);
		}
	}
	m_Model.Set(mesh, m_ActiveChunk);
	return true;
}

Faces Chunk::GetNeighbouringBlocks(glm::vec3 position)
{
	Faces localFaces;

	glm::vec3 offsetPosition = position + glm::vec3(1, 0, 0);
	int block = GetBlock(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.right] = true;
	}
	offsetPosition = position + glm::vec3(-1, 0, 0);
	block = GetBlock(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.left] = true;
	}

	offsetPosition = position + glm::vec3(0, 1, 0);
	block = GetBlock(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.top] = true;
	}

	offsetPosition = position + glm::vec3(0, -1, 0);
	block = GetBlock(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.bottom] = true;
	}

	offsetPosition = position + glm::vec3(0, 0, -1);
	block = GetBlock(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.back] = true;
	}

	offsetPosition = position + glm::vec3(0, 0, 1);
	block = GetBlock(offsetPosition);
	if (block == 0) {
		localFaces.faces[localFaces.front] = true;
	}

	return localFaces;
}

const int Chunk::GetIndex(glm::vec3 position)
{
	if (position.x < 0 || position.x >= CHUNK_SIZE || position.z < 0 || position.z >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT) return -1;

	return position.x + position.y * (CHUNK_HEIGHT)+position.z * (CHUNK_SIZE);
}

const int Chunk::GetBlock(glm::vec3 position)
{
	if (position.y >= CHUNK_HEIGHT || position.y < 0) return 0;

	unsigned int index = 0;
	int edge = -1;
	int edgeZ = 0;
	if (position.x <= -1) {
		index = position.z;
		edge = 3;
	}
	else if (position.x >= CHUNK_SIZE) {
		index = position.z;
		edge = 1;
	}
	else if (position.z <= -1) {
		index = position.x;
		edge = 2;
	}
	else if (position.z >= CHUNK_SIZE) {
		index = position.x;
		edge = 0;
	}

	if (edge != -1) {
		unsigned char block = m_BlocksEdge[edge][index][(int)position.y];
		if (block == 0) {
			return block;
		}
	}

	index = GetIndex(position);
	if (index == -1) {
		return -1;
	}
	int id = m_Blocks[index];

	return id;
}



