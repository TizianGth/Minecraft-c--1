#pragma once

#include "Mesh.h"

class Shapes {
public: 
	static Mesh Box(float size);
	static Mesh Box(float x, float y, float z, float size);
};