#include "Model.h"

Model::~Model()
{
    if(m_Vb != nullptr) delete m_Vb;
    if (m_Ib != nullptr) delete m_Ib;
    if (m_Va != nullptr) delete m_Va;
    if (m_Layout != nullptr) delete m_Layout;
}

void Model::addVB()
{
    if (m_Vb == nullptr) {
        m_Vb = new VertexBuffer();
    }

    m_Vb->Set(m_Mesh.vertices.data(), sizeof(float) * m_Mesh.vertices.size());
}

void Model::addIB()
{
    if (m_Ib == nullptr) {
        m_Ib = new IndexBuffer();
    }
    m_Ib->Set(m_Mesh.indices.data(), m_Mesh.indices.size());
}

void Model::addVA()
{
    if (m_Va == nullptr) {
        m_Va = new VertexArray();
    }
    m_Va->AddBuffer(*m_Vb, *m_Layout);
}

void Model::addLayout()
{
    if (m_Layout == nullptr) {
        m_Layout = new VertexBufferLayout();
    }
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(1);
}


void Model::Set(Mesh& mesh)
{
    m_Mesh = mesh;
}

bool Model::isValid()
{
    return !(m_Layout == nullptr && m_Va == nullptr && m_Ib == nullptr && m_Vb == nullptr);
}


