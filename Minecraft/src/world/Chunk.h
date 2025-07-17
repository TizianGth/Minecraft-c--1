#pragma once

#include "Faces.h"
#include "Model.h"
#include "PerlinNoise.hpp"
#include <thread>
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include <unordered_map>
#include "GeneratorSettings.h"

#include "Line.h"

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 256;

struct hash
{
	size_t operator()(const glm::ivec2& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
	}

	bool operator()(const glm::ivec2& a, const glm::ivec2& b)const
	{
		return a.x == b.x && a.y == b.y;
	}
};



class Chunk
{
public:
	unsigned char m_Blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT] = {};

	Mesh m_Mesh;
	Model* m_Model = nullptr;

	std::unordered_map<glm::ivec2, Chunk*, hash> m_NeighbourChunks;

	glm::mat4 m_ModelMat4 = glm::mat4(1.0f);

	bool m_Hide = true;
	bool m_Bound = false;
	bool m_Generated = false;
	bool m_readyToRender = false;

	int positionX, positionY;

	Chunk(int x, int y);
	~Chunk();

	void Changeblock(int x, int y, int z, unsigned char block);

	void Generate();

	void Fill();
	void GenerateMesh();
	void AddToMesh(int x, int y, int z, Faces& faces);
	void Bind(Model* model);

	Faces CheckSurroundings(int x, int y, int z);

	const int GetIndex(int x, int y, int z);
	const Settings::BlockTypes Getblock(int x, int y, int z);

private:
	unsigned int indicesOffset = 0;

	void AddVerticesToMesh(std::vector<float>& vertices, int x, int y, int z);
	void AddTextCoordsToMesh(std::vector<float>& texCoords);
	void AddTextMaterialsToMesh(std::vector<unsigned short>& materials, unsigned int materialID);
	void AddTextIndicesToMesh(std::vector<unsigned int>& indices, int offset,Faces& faces);

	int GetPerlinHeight(int x, int y);

	const siv::PerlinNoise::seed_type m_Seed = 123456u;

	const siv::PerlinNoise m_Perlin{ m_Seed };


};


