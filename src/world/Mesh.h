#pragma once

#include <vector>
#include "glm/glm.hpp"


struct Mesh {
public:
	inline Mesh() {};
	inline Mesh(Mesh* mesh) {
		verticesPosition = mesh->verticesPosition;
		texCoords = mesh->texCoords;
		materialID = mesh->materialID;
		indices = mesh->indices;
	};

	inline ~Mesh() {};

	std::vector<float> verticesPosition;
	std::vector<char> texCoords;
	std::vector<unsigned char> materialID;
	std::vector<unsigned int> indices;
};