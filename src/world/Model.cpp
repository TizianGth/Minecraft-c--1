#include "Model.h"

Model::~Model()
{
	delete m_Vb;
	delete m_Ib;
	delete m_Va;
	delete m_Layout;
	delete m_Mesh;
}

void Model::addVB()
{
	delete m_Vb;
	m_Vb = new VertexBuffer();

	m_Vb->Set(m_Mesh->verticesPosition.data(), sizeof(float) * m_Mesh->verticesPosition.size());
}

void Model::addIB()
{
	delete m_Ib;
	m_Ib = new IndexBuffer();

	m_Ib->Set(m_Mesh->indices.data(), m_Mesh->indices.size());
}

void Model::addVA()
{
	delete m_Va;
	m_Va = new VertexArray();

	m_Va->AddBuffer(*m_Vb, *m_Layout);
}

void Model::addLayout()
{
	delete m_Layout;
	m_Layout = new VertexBufferLayout();

	m_Layout->Push<float>(3);
	m_Layout->Push<float>(3);
	m_Layout->Push<float>(1);
}

void Model::Bind()
{
	addLayout();
	addVB();
	addIB();
	addVA();

	if (!m_Active) {
		delete m_Mesh;
		m_Mesh = nullptr;
	}

	m_Bound = true;

}


void Model::Set(Mesh* mesh, bool active)
{
	delete m_Mesh;
	m_Mesh = mesh;
	m_Active = active;
}

bool Model::isValid()
{
	return m_Layout != nullptr && m_Va != nullptr && m_Ib != nullptr && m_Vb != nullptr;
}


