#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Model {
public:
	Mesh m_Mesh;
	VertexBuffer m_Vb;
	IndexBuffer m_Ib;
	VertexArray m_Va;
	VertexBufferLayout m_Layout;

	void addVB();
	void addIB();
	void addVA();
	void Set(Mesh &mesh);
};