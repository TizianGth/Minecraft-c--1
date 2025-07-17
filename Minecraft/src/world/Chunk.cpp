#include "Chunk.h"
#include <thread>

#include <chrono>
#include <iostream>

Chunk::Chunk(int x, int y)
{
	positionX = x, positionY = y;

	m_ModelMat4 = glm::translate(glm::mat4(1.0f), glm::vec3(x * CHUNK_SIZE, 0, y * CHUNK_SIZE));
	Fill();
}

Chunk::~Chunk()
{
	delete m_Model;
}

void Chunk::Changeblock(int x, int y, int z, unsigned char block)
{
	int index = GetIndex(x, y, z);
	m_Blocks[index] = block;
}

void Chunk::Generate()
{
	indicesOffset = 0;
	m_Generated = false;
	m_Bound = false;

	m_Mesh.Clear();
	//delete m_Model;
	m_Model = nullptr;

	GenerateMesh();

	m_Generated = true;
}

void Chunk::Fill()
{
	// Fill block array with grass blocks based on perlin noise height map, fill rest with air
	int maxY = 0, height = 0;

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			height = GetPerlinHeight(x, z);
			height = std::max(height, (int)Biomes::Plains::WATER);
			m_HighestBlock = std::max(m_HighestBlock, height);

			for (int y = height; y >= 0; y--) {
				int index = GetIndex(x, y, z);
				m_Blocks[index] = Settings::Settings::BlockTypes::GRASS;

				if (y <= Biomes::Plains::WATER) {
					m_Blocks[index] = Settings::Settings::BlockTypes::WATER;
				}
				else if((y == Biomes::Plains::SAND)){
					m_Blocks[index] = Settings::Settings::BlockTypes::SAND;
				}

				if (y < height) {
					if (m_Blocks[GetIndex(x, y+1, z)] == Settings::Settings::BlockTypes::GRASS) {
						m_Blocks[index] = Settings::Settings::BlockTypes::DIRT;
					}
				}
			}
		}
	}

	m_FrustumBounds = AABB(glm::vec3(-1,-1,-1), glm::vec3(CHUNK_SIZE+1, m_HighestBlock+1, CHUNK_SIZE+1));
}

void Chunk::GenerateMesh()
{
	// Check for every block in chunk neighbouring block and if face should be rendered or not
	// then add vertices/indices to mesh according to faces
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				auto f = CheckSurroundings(x, y, z);
				AddToMesh(x, y, z, f);
			}
		}
	}
}

Faces Chunk::CheckSurroundings(int x, int y, int z)
{
	Faces faces;

	// Loop through every face
	for (int i = 0; i < 6; i++) {

		// Get index of the block facing in the face direction
		auto dir = Face::m_Directions[i];
		int _x = std::get<0>(dir) + x;
		int _y = std::get<1>(dir) + y;
		int _z = std::get<2>(dir) + z;
		int index = GetIndex(_x, _y, _z);

		// Only draw face when adjacent block = air (id: 0)
		if (index != -1 && m_Blocks[index] == Settings::BlockTypes::AIR) { //&& Settings::RenderRegardless.contains((Settings::BlockTypes)m_Blocks[index])) {
			faces.m_Faces[i] = true;
		}
		// or when block in neighbouring chunk that adjacent to an air block
		else if (index == -1 && _y <= CHUNK_HEIGHT && _y >= 0) {
			if (std::get<1>(dir) != 0)
				continue;


			glm::ivec2 offs = glm::ivec2(std::get<0>(dir), std::get<2>(dir));
			if (m_NeighbourChunks[offs] == nullptr)
				continue;

			unsigned char  block = m_NeighbourChunks[offs]->Getblock(_x - CHUNK_SIZE * std::get<0>(dir), _y, _z - CHUNK_SIZE * std::get<2>(dir));
			if (block != Settings::BlockTypes::INVALID && block == Settings::BlockTypes::AIR) { //Settings::RenderRegardless.contains((Settings::BlockTypes)block)) {
				faces.m_Faces[i] = true;
			}
		} 
	}
	return faces;
}

void Chunk::AddToMesh(int x, int y, int z, Faces& faces)
{
	// Don't add to mesh when theres no face to render or block = air
	int index = GetIndex(x, y, z);
	if (m_Blocks[index] == Settings::BlockTypes::AIR || faces.Count() == 0) {
		return;
	}

	// Add vertices and indices to mesh
	AddVerticesToMesh(m_Mesh.verticesPosition, x, y, z);
	AddTextCoordsToMesh(m_Mesh.texCoords);
	AddTextMaterialsToMesh(m_Mesh.materialID, m_Blocks[index]);
	// offset*24 because each vertex contains 24 float values
	AddTextIndicesToMesh(m_Mesh.indices, indicesOffset*24, faces);

	// offset == amount of blocks already added
	indicesOffset++;
}

void Chunk::Bind(Model* model)
{
	m_Model = model;
	m_Model->AddData(m_Mesh);

	m_Bound = true;
	m_readyToRender = true;
}

const int Chunk::GetIndex(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT) 
		return -1;

	return x + y * (CHUNK_SIZE) + z * (CHUNK_SIZE* CHUNK_HEIGHT);
}

const Settings::BlockTypes Chunk::Getblock(int x, int y, int z)
{
	int index = GetIndex(x, y, z);
	if (index == -1)
		return Settings::BlockTypes::INVALID;
	return (Settings::BlockTypes)m_Blocks[index];
}

void Chunk::AddVerticesToMesh(std::vector<float>& vertices, int x, int y, int z)
{
	std::vector<float> verts = {
		(float)x		, (float)y			, (float)(z + 1),
		(float)(x + 1)	, (float)y			, (float)(z + 1),
		(float)(x + 1)	, (float)(y + 1)	, (float)(z + 1),
		(float)x		, (float)(y + 1)	, (float)(z + 1),
															
		(float)x		, (float)y			, (float)z		,
		(float)(x + 1)	, (float)y			, (float)z		,
		(float)(x + 1)	, (float)(y + 1)	, (float)z		,
		(float)x		, (float)(y + 1)	, (float)z		,

		(float)x		, (float)(y + 1), (float) z		,
		(float)(x + 1)	, (float)(y + 1), (float) z		,
		(float)(x + 1)	, (float)(y + 1), (float)(z + 1),
		(float)x		, (float)(y + 1), (float)(z + 1),

		(float)x		, (float)y, (float)z		,
		(float)(x + 1)	, (float)y, (float)z		,
		(float)(x + 1)	, (float)y, (float)(z + 1)	,
		(float)x		, (float)y, (float)(z + 1)	,

		(float)(x + 1)	, (float)y		,(float) z		,
		(float)(x + 1)	, (float)y		,(float)(z + 1)	,
		(float)(x + 1)	, (float)(y + 1),(float)(z + 1)	,
		(float)(x + 1)	, (float)(y + 1),(float) z		,
														
		(float) x		, (float)y		,(float) z		,
		(float) x		, (float)y		,(float)(z + 1)	,
		(float) x		, (float)(y + 1),(float)(z + 1)	,
		(float) x		, (float)(y + 1),(float) z		,

	};

	vertices.insert(vertices.end(), verts.begin(), verts.end());
}

void Chunk::AddTextCoordsToMesh(std::vector<float>& texCoords)
{
	std::vector<float> coords = {

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
			  		
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
					
		0.0f, 0.0f, 2.0f,
		1.0f, 0.0f, 2.0f,
		1.0f, 1.0f, 2.0f,
		0.0f, 1.0f, 2.0f,
					
		0.0f, 0.0f, 3.0f,
		1.0f, 0.0f, 3.0f,
		1.0f, 1.0f, 3.0f,
		0.0f, 1.0f, 3.0f,

		0.0f, 0.0f, 4.0f,
		1.0f, 0.0f, 4.0f,
		1.0f, 1.0f, 4.0f,
		0.0f, 1.0f, 4.0f,
					
		0.0f, 0.0f, 5.0f,
		1.0f, 0.0f, 5.0f,
		1.0f, 1.0f, 5.0f,
		0.0f, 1.0f, 5.0f,
	};
	texCoords.insert(texCoords.end(), coords.begin(), coords.end());
}

void Chunk::AddTextMaterialsToMesh(std::vector<unsigned short>& materials, unsigned int materialID)
{
	std::vector<unsigned short> mats = {
		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,

		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,

		(unsigned short)(materialID),
		(unsigned short)(materialID),
		(unsigned short)(materialID),
		(unsigned short)(materialID),
								
		(unsigned short)(materialID),
		(unsigned short)(materialID),
		(unsigned short)(materialID),
		(unsigned short)(materialID),

		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,

		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,
		(unsigned short)materialID,
	};
	materials.insert(materials.end(), mats.begin(), mats.end());
}

void Chunk::AddTextIndicesToMesh(std::vector<unsigned int>& indices, int offset, Faces& faces)
{
	std::vector<int> frontIndices = {
		// Front
		 1 + offset,2 + offset,0 + offset,
		 3 + offset, 0 + offset,2 + offset
	};
	std::vector<int> backIndices = {
		// Back
		5 + offset,4 + offset,6 + offset,
		7 + offset,6 + offset,4 + offset,
	};
	std::vector<int> topIndices = {
		// Top
		9 + offset, 8 + offset, 10 + offset,
		11 + offset, 10 + offset, 8 + offset
	};
	std::vector<int> bottomIndices = {
		// Bottom
		12 + offset, 13 + offset, 14 + offset,
		14 + offset, 15 + offset, 12 + offset
	};
	std::vector<int> rightIndices = {
		// Right
		17 + offset, 16 + offset, 18 + offset,
		19 + offset, 18 + offset, 16 + offset
	};
	std::vector<int> leftIndices = { //
		// Left
		21 + offset, 22 + offset, 20 + offset,
		23 + offset, 20 + offset, 22 + offset
	};

	if (faces.m_Faces[Face::front]) {
		indices.insert(indices.end(), frontIndices.begin(), frontIndices.end());
	}
	if (faces.m_Faces[Face::back]) {
		indices.insert(indices.end(), backIndices.begin(), backIndices.end());
	}
	if (faces.m_Faces[Face::top]) {
		indices.insert(indices.end(), topIndices.begin(), topIndices.end());
	}
	if (faces.m_Faces[Face::bottom]) {
		indices.insert(indices.end(), bottomIndices.begin(), bottomIndices.end());
	}
	if (faces.m_Faces[Face::right]) {
		indices.insert(indices.end(), rightIndices.begin(), rightIndices.end());
	}
	if (faces.m_Faces[Face::left]) {
		indices.insert(indices.end(), leftIndices.begin(), leftIndices.end());
	}
}

int Chunk::GetPerlinHeight(int x, int y)
{
	double a = 0.1;
	int octavesBig = 2;
	double scaleBig = 0.03;
	double multiplierBig = 200.0;

	double big = (m_Perlin.octave2D_01(((double)x + ((double)(CHUNK_SIZE)*positionX)) * a * scaleBig, ((double)y + ((double)(CHUNK_SIZE)*positionY)) * a * scaleBig, octavesBig) * multiplierBig);



	//double a = 0.1;
	int octavesSmall = 6;
	double scaleSmall = 0.1;
	double multiplierSmall = 0.45;

	double small = (m_Perlin.octave2D_01(((double)x + ((double)(CHUNK_SIZE)*positionX)) * a * scaleSmall, ((double)y + ((double)(CHUNK_SIZE)*positionY)) * a * scaleSmall, octavesSmall) * multiplierSmall);


	//int river = std::clamp((int)((m_Perlin.octave2D_01(((double)x + ((double)(CHUNK_SIZE)*positionX)) * a * 2.0f, ((double)y + ((double)(CHUNK_SIZE)*positionY)) * a * 2.0f, 0) * f) - 2), 0, (int)Biomes::Plains::BlockStartHeight::SAND);

	return (int)(big*small);
}
