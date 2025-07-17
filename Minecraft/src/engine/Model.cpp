#include "Model.h"
#include <iostream>
#include <chrono>

Model::Model(const Mesh& mesh)
{
	AddData(mesh);
}

Model::Model()
{

}

Model::~Model()
{
	DeleteData();
}

void Model::AddData(const Mesh& mesh)
{
	GenVAO();
	AddVBO(3, mesh.verticesPosition);
	AddVBO(3, mesh.texCoords);
	AddVBO(1, mesh.materialID);
	AddEBO(mesh.indices);
}

void Model::GenVAO()
{
	if (m_Vao != 0)
		DeleteData();

	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);
}

void Model::AddEBO(const std::vector<unsigned int>& indices)
{
	unsigned int ebo;
	m_IndicesCount = static_cast<unsigned int>(indices.size());

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		indices.data(), GL_DYNAMIC_DRAW);

	m_Buffers.push_back(ebo);
}


void Model::AddVBO(int dimensions, const std::vector<float>& data)
{
	unsigned int vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(unsigned int),
		data.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(static_cast<unsigned int>(m_VboCount),
		dimensions, GL_FLOAT, GL_FALSE, 0, (unsigned int*)0);

	glEnableVertexAttribArray(static_cast<unsigned int>(m_VboCount++));

	m_Buffers.push_back(vbo);
}
void Model::AddVBO(int dimensions, const std::vector<unsigned short>& data)
{
	unsigned int vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(unsigned short),
		data.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(static_cast<unsigned int>(m_VboCount),
		dimensions, GL_UNSIGNED_SHORT, GL_FALSE, 0, (unsigned int*)0);

	glEnableVertexAttribArray(static_cast<unsigned int>(m_VboCount++));

	m_Buffers.push_back(vbo);
}

void Model::BindVAO() const
{
	glBindVertexArray(m_Vao);
}

void Model::DeleteData()
{
	if (m_Vao != 0)
		glDeleteVertexArrays(1, &m_Vao);
	if (m_Buffers.size() > 0)
		glDeleteBuffers(static_cast<GLsizei>(m_Buffers.size()), m_Buffers.data());

	m_Buffers.clear();

	m_Vao = 0;
	m_IndicesCount = 0, m_VboCount = 0;
}



