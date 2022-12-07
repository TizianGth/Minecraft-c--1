#include "Chunk.h"
#include <iostream>
#include <stdint.h>
#include <chrono>

//TODO: CHUNK AND WORLD POSITION CONVERSION
Chunk::Chunk()
{

}
Chunk::~Chunk()
{

}

std::vector<int> Chunk::ConvertPositionToVertices(glm::vec3& position, int materialID) {
    int x = position.x;
    int y = position.y;
    int z = position.z;
    std::vector<int> result = {
       x,     y,     (z + 1), -1, -1, 1, materialID,
       (x + 1), y,    (z + 1),  1, -1, 1, materialID,
       (x + 1), (y + 1), (z + 1),  1,  1, 1, materialID,
       x,     (y + 1), (z + 1), -1,  1, 1, materialID,

       x,     y,     z, -1, -1, -1, materialID,
       (x + 1), y,     z,  1, -1, -1, materialID,
       (x + 1), (y + 1), z,  1,  1, -1, materialID,
       x,     (y + 1), z, -1,  1, -1, materialID
    };

    return result;
}

std::vector<int> Chunk::ConvertPositionToIndex(int blockCount, Faces faces)
{    std::vector<int> frontIndices = {
            // Front
             0 + (8 * blockCount),1 + (8 * blockCount),2 + (8 * blockCount),
             2 + (8 * blockCount),3 + (8 * blockCount),0 + (8 * blockCount)
        };
        std::vector<int> backIndices = {
             // Back
             4 + (8 * blockCount),5 + (8 * blockCount),6 + (8 * blockCount),
             6 + (8 * blockCount),7 + (8 * blockCount),4 + (8 * blockCount),
        };
        std::vector<int> rightIndices = {
             // Right
             1 + (8 * blockCount), 5 + (8 * blockCount), 6 + (8 * blockCount),
             6 + (8 * blockCount), 2 + (8 * blockCount), 1 + (8 * blockCount)
        };
        std::vector<int> leftIndices = {
             // Left
             0 + (8 * blockCount), 4 + (8 * blockCount), 7 + (8 * blockCount),
             7 + (8 * blockCount), 3 + (8 * blockCount), 0 + (8 * blockCount)
        };
        std::vector<int> topIndices = {
             // Top
             2 + (8 * blockCount), 3 + (8 * blockCount), 7 + (8 * blockCount),
             7 + (8 * blockCount), 2 + (8 * blockCount), 6 + (8 * blockCount)
        };
        std::vector<int> bottomIndices = {
             // Bottom
             1 + (8 * blockCount), 5 + (8 * blockCount), 4 + (8 * blockCount),
             4 + (8 * blockCount), 1 + (8 * blockCount), 0 + (8 * blockCount)
        };


    std::vector<int> result;
   // result.reserve(faces.Count() * 6);

    if (faces.faces[faces.front]) {
        result.insert(result.end(), frontIndices.begin(), frontIndices.end());
    }
    if (faces.faces[faces.back]) {
        result.insert(result.end(), backIndices.begin(), backIndices.end());
    }
    if (faces.faces[faces.right]) {
        result.insert(result.end(), rightIndices.begin(), rightIndices.end());
    }
    if (faces.faces[faces.left]) {
        result.insert(result.end(), leftIndices.begin(), leftIndices.end());
    }
    if (faces.faces[faces.top]) {
        result.insert(result.end(), topIndices.begin(), topIndices.end());
    }
    if (faces.faces[faces.bottom]) {
        result.insert(result.end(), bottomIndices.begin(), bottomIndices.end());
    }

    return result;
}

void Chunk::FillUpTest()
{
    // leave 1 block on each side otherwise, faces on the outside wont be rendered because they touch the other chunk
    // TODO: debug why if face touches other chunk sometimes itll still get rendered
    glm::vec3 position = glm::vec3(0, 0, 0);
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            for (int y = 0; y < 3; y++) {
                position = glm::vec3(x, y, z);
                m_blocks[ConvertVector3ToIndex(position)] = 1;
            }
        }
    }

    for (int x = 0; x < 15; x++) {
        for (int z = 0; z < 15; z++) {
            for (int y = 3; y < 4; y++) {
                position = glm::vec3(x, y, z);
                m_blocks[ConvertVector3ToIndex(position)] = 2;
            }
        }
    }

}

/// <summary>
/// Fill whole chunk with air blocks (blockId = 0)
/// </summary>
void Chunk::Generate()
{
    // 1. fill first row  (x,0,0) and keep track when finished then currentZLevel += 1, then when both finished currentYLevel += 1;
    int progressToNextZ = 0;    // same as x
    int progressToNextY = 0;
    int currentZLevel = 0;      // same as z
    int currentYLevel = 0;      // sane as y

    // this is the data that will be saved in the end
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
        m_blocks[i] = 0;

        glm::vec3 position(progressToNextZ, currentYLevel, currentZLevel);
        m_localBlockPositions[i] = position;

        progressToNextZ++;
        progressToNextY++;

        if (progressToNextZ == CHUNK_SIZE) {
            progressToNextZ = 0;
            currentZLevel++;
        }

        if (progressToNextY == CHUNK_SIZE * CHUNK_SIZE) {
            progressToNextY = 0;
            progressToNextZ = 0;
            currentZLevel = 0;
            currentYLevel++;
        }

    } 
}



void Chunk::GenerateMeshes()
{

    m_Mesh.vertices.clear();
    m_Mesh.indices.clear();

    // Accounts for Air blocks, because they shouldnt be counted to index
    int minus = 0;

    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
        if (m_blocks[i] == 0) {
            minus++;
            continue;
        } 
        glm::vec3 position = m_localBlockPositions[i];
        int materialID = m_blocks[i];
        Faces faces = GetNeighbouringBlocks(position);

        if (faces.Count() == 6) continue;

        std::vector<int> newVertices = ConvertPositionToVertices(position, materialID);
        int newVerticesLength = newVertices.size();

        m_Mesh.vertices.reserve(newVerticesLength);

        for (int v = 0; v < newVerticesLength; v++) {
            m_Mesh.vertices.emplace_back(newVertices[v]);
        }

        std::vector<int> newIndices = ConvertPositionToIndex(i-minus, faces);
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


/// <summary>
/// creates "seams" on chunks, to remove them youd need a global block buffer or check sorounding chunks. 
/// Can be important when y is getting greater. Not worth it atm
/// </summary>

Faces Chunk::GetNeighbouringBlocks(glm::vec3& position)
{
    Faces localFaces;

    glm::vec3 offsetPosition = position + glm::vec3(1, 0, 0);
    int index = ConvertVector3ToIndex(offsetPosition);
    if (position.x >= CHUNK_SIZE || position.y >= CHUNK_HEIGHT || position.z >= CHUNK_SIZE ||index != -1 && m_blocks[index] == 0) {
        localFaces.faces[localFaces.right] = true;
    }

    offsetPosition = position + glm::vec3(-1, 0, 0);
    index = ConvertVector3ToIndex(offsetPosition);
    if (index == -1 || m_blocks[index] == 0) {
        localFaces.faces[localFaces.left] = true;
    }

    offsetPosition = position + glm::vec3(0, 1, 0);
    index = ConvertVector3ToIndex(offsetPosition);
    if (index == -1 || m_blocks[index] == 0) {
        localFaces.faces[localFaces.top] = true;
    }

    offsetPosition = position + glm::vec3(0, -1, 0);
    index = ConvertVector3ToIndex(offsetPosition);
    if (index == -1 || m_blocks[index] == 0) {
        localFaces.faces[localFaces.bottom] = true;
    }

    offsetPosition = position + glm::vec3(0, 0, -1);
    index = ConvertVector3ToIndex(offsetPosition);
    if (index == -1 || m_blocks[index] == 0) {
        localFaces.faces[localFaces.back] = true;
    }

    offsetPosition = position + glm::vec3(0, 0, 1);
    index = ConvertVector3ToIndex(offsetPosition);
    if (index == -1 || m_blocks[index] == 0) {
        localFaces.faces[localFaces.front] = true;
    }

    offsetPosition = glm::vec3(-1, 0, 0);

    return localFaces;
}

int Chunk::ConvertVector3ToIndex(glm::vec3& position)
{
    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT || position.z < 0 || position.z >= CHUNK_SIZE) return -1;

    // formula to convert vector3 to index = x + y*(CHUNK_HEIGHT) + z*(CHUNK_SIZE)
    return position.x + position.y * (CHUNK_HEIGHT) + position.z * (CHUNK_SIZE);
}



