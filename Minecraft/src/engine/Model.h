#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

class Model {
public:
	Model(const Mesh& mesh);
	Model();

	~Model();

	void AddData(const Mesh& mesh);
	void ReplaceData(const Mesh& mesh);

	void GenVAO();
	void AddEBO(const std::vector<unsigned int>& indices);

	void AddVBO(int dimensions, const std::vector<float>& data);
	void AddVBO(int dimensions, const std::vector<unsigned short>& data);
	void BindVAO() const;

	void DeleteData();

	unsigned int m_Vao = 0;
	unsigned int m_IndicesCount = 0, m_VboCount = 0;

private:
	std::vector<unsigned int> m_Buffers;
};
