#include "Chunk.h"

Chunk::Chunk(vector3 positions[ChunkSize])
{
	for (int i = 0; i < ChunkSize; i++) {
        Mesh mesh;
		float x = positions[i].x();
		float y = positions[i].y();
		float z = positions[i].z();

       mesh.vertices = {
	   -1 + x, -1 + y, 1 + z, -1, -1, 1, 0,
		1 + x, -1 + y, 1 + z,  1, -1, 1, 0,
		1 + x,  1 + y, 1 + z,  1,  1, 1, 0,
	   -1 + x,  1 + y, 1 + z, -1,  1, 1, 0,

	   -1 + x, -1 + y, -1 + z, -1, -1, -1, 0,
		1 + x, -1 + y, -1 + z,  1, -1, -1, 0,
		1 + x,  1 + y, -1 + z,  1,  1, -1, 0,
	   -1 + x,  1 + y, -1 + z, -1,  1, -1, 0
		};

       mesh.indices = {
            // Front
             0,1,2,
             2,3,0,

             // Back
             4,5,6,
             6,7,4,

             // Right
             1, 5, 6,
             6, 2, 1,

             // Left
             0, 4, 7,
             7, 3, 0,

             // Top
             2, 3, 7,
             7, 2, 6,

             // Bottom
             1, 5, 4,
             4, 1, 0
        };

       m_Models[i].Set(mesh);
	}
}

Chunk::~Chunk()
{
}

void Chunk::Set()
{
    for (int i = 0; i < ChunkSize; i++) {
        m_Models[i].addVB();
        m_Models[i].addIB();
        m_Models[i].addVA();
    }
}


