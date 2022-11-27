#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glad/gl.h"
#include "vector.h"


struct Mesh {
public:
	std::vector<float> vertices;
	std::vector<int> indices;
};