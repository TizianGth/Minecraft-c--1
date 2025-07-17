#pragma once

#include <vector>


struct Mesh {
public:
	std::vector<float> verticesPosition;
	std::vector<float> texCoords;
	std::vector<unsigned short> materialID;
	std::vector<unsigned int> indices;

	void Clear() {
		verticesPosition.clear();
		texCoords.clear();
		materialID.clear();
		indices.clear();
	}
};