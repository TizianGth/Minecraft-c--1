#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glad/gl.h"


struct Mesh {
public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};