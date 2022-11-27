#include "Chunk.h"


//TODO: CHUNK AND WORLD POSITION CONVERSION
Chunk::Chunk(std::vector<Blocktype>& blocks)
{
    int length = blocks.size();
    for (int i = 0; i < length; i++) {

        vector3Int position = blocks[i].m_Position;

        m_Blocks[position.x()][position.y()][position.z()].Set(position, 0);

        std::vector<float> newVertices = ConvertPositionToVertices(position);
        int newVerticesLength = newVertices.size();

        m_Mesh.vertices.reserve(newVerticesLength);

        for (int v = 0; v < newVerticesLength; v++) {
            m_Mesh.vertices.emplace_back(newVertices[v]);
        }

        std::vector<int> newIndices = ConvertPositionToIndex(i);
        int newIndicesLength = newIndices.size();

        m_Mesh.indices.reserve(newIndicesLength);

        for (int v = 0; v < newIndicesLength; v++) {
            m_Mesh.indices.emplace_back(newIndices[v]);
        }

    }


    m_Model.Set(m_Mesh);
    m_Model.addVB();
    m_Model.addIB();
    m_Model.addVA();
}

std::vector<float> Chunk::ConvertPositionToVertices(vector3Int position) {
    float x = position.x();
    float y = position.y();
    float z = position.z();
    std::vector<float> result = {
       x,     y,     z + 1, -1, -1, 1, 0,
       x + 1, y,     z + 1,  1, -1, 1, 0,
       x + 1, y + 1, z + 1,  1,  1, 1, 0,
       x,     y + 1, z + 1, -1,  1, 1, 0,

       x,     y,     z, -1, -1, -1, 0,
       x + 1, y,     z,  1, -1, -1, 0,
       x + 1, y + 1, z,  1,  1, -1, 0,
       x,     y + 1, z, -1,  1, -1, 0
    };

    return result;
}

std::vector<int> Chunk::ConvertPositionToIndex(int blockCount)
{
    std::vector<int> result = {
        // Front
         0 + (8 * blockCount),1 + (8 * blockCount),2 + (8 * blockCount),
         2 + (8 * blockCount),3 + (8 * blockCount),0 + (8 * blockCount),

         // Back
         4 + (8 * blockCount),5 + (8 * blockCount),6 + (8 * blockCount),
         6 + (8 * blockCount),7 + (8 * blockCount),4 + (8 * blockCount),

         // Right
         1 + (8 * blockCount), 5 + (8 * blockCount), 6 + (8 * blockCount),
         6 + (8 * blockCount), 2 + (8 * blockCount), 1 + (8 * blockCount),

         // Left
         0 + (8 * blockCount), 4 + (8 * blockCount), 7 + (8 * blockCount),
         7 + (8 * blockCount), 3 + (8 * blockCount), 0 + (8 * blockCount),

         // Top
         2 + (8 * blockCount), 3 + (8 * blockCount), 7 + (8 * blockCount),
         7 + (8 * blockCount), 2 + (8 * blockCount), 6 + (8 * blockCount),

         // Bottom
         1 + (8 * blockCount), 5 + (8 * blockCount), 4 + (8 * blockCount),
         4 + (8 * blockCount), 1 + (8 * blockCount), 0 + (8 * blockCount)
    };


    return result;
}


Chunk::~Chunk()
{
}

void Chunk::Set()
{

}


