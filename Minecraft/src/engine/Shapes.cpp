#include "Shapes.h"

Mesh Shapes::Box(float size)
{
	Mesh mesh;

	mesh.verticesPosition = {
		-0.5f * size, -0.5f * size,  0.5f * size, -1, -1,  1, 0, // 0
		 0.5f * size, -0.5f * size,  0.5f * size,  1, -1,  1, 0, // 1
		 0.5f * size,  0.5f * size,  0.5f * size,  1,  1,  1, 0, // 2
		-0.5f * size,  0.5f * size,  0.5f * size, -1,  1,  1, 0, // 3
		-0.5f * size, -0.5f * size, -0.5f * size, -1, -1, -1, 0, // 4
		 0.5f * size, -0.5f * size, -0.5f * size,  1, -1, -1, 0, // 5
		 0.5f * size,  0.5f * size, -0.5f * size,  1,  1, -1, 0, // 6
		-0.5f * size,  0.5f * size, -0.5f * size, -1,  1, -1, 0  // 7
	};
	mesh.indices = {
		0,1,2,	4,5,6,
		2,3,0,	6,7,4,

		1,5,6,	0,4,7,
		6,2,1,	7,3,0,

		2,3,7,	1,5,4,
		7,2,6,	4,1,0
	};

	return mesh;
}

Mesh Shapes::Box(float x, float y, float z, float size)
{
	Mesh mesh;

	mesh.verticesPosition = {
		(-0.5f + x) * size, (-0.5f + y) * size, ( 0.5f + z) * size, -1, -1,  1, 0, // 0
		( 0.5f + x) * size, (-0.5f + y) * size, ( 0.5f + z) * size,  1, -1,  1, 0, // 1
		( 0.5f + x) * size, ( 0.5f + y) * size, ( 0.5f + z) * size,  1,  1,  1, 0, // 2
		(-0.5f + x) * size, ( 0.5f + y) * size, ( 0.5f + z) * size, -1,  1,  1, 0, // 3
		(-0.5f + x) * size, (-0.5f + y) * size, (-0.5f + z) * size, -1, -1, -1, 0, // 4
		( 0.5f + x) * size, (-0.5f + y) * size, (-0.5f + z) * size,  1, -1, -1, 0, // 5
		( 0.5f + x) * size, ( 0.5f + y) * size, (-0.5f + z) * size,  1,  1, -1, 0, // 6
		(-0.5f + x) * size, ( 0.5f + y) * size, (-0.5f + z) * size, -1,  1, -1, 0  // 7
	};
	mesh.indices = {
		0,1,2,	4,5,6,
		2,3,0,	6,7,4,

		1,5,6,	0,4,7,
		6,2,1,	7,3,0,

		2,3,7,	1,5,4,
		7,2,6,	4,1,0
	};

	return mesh;
}
