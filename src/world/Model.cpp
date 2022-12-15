#include "Model.h"

void Model::addVB()
{
    m_Vb.Set(m_Mesh.vertices.data(), sizeof(float) * m_Mesh.vertices.size());
}

void Model::addIB()
{
    m_Ib.Set(m_Mesh.indices.data(), m_Mesh.indices.size());
}

void Model::addVA()
{
    m_Va.AddBuffer(m_Vb, m_Layout);
}

void Model::Set(Mesh& mesh)
{
    m_Layout.Push<float>(3);
    m_Layout.Push<float>(3);
    m_Layout.Push<float>(1);
    m_Mesh = mesh;
}


