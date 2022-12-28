#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Model {
public:
	Model() {};
	~Model();

	Mesh* m_Mesh;
	VertexBuffer *m_Vb = nullptr;
	IndexBuffer *m_Ib = nullptr;
	VertexArray *m_Va = nullptr;
	VertexBufferLayout *m_Layout = nullptr;

	bool m_Active;

	void addVB();
	void addIB();
	void addVA();
	void addLayout();
	void Bind();
	void Set(Mesh* mesh, bool active);

	bool isValid();
};