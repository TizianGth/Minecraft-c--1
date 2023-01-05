#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Model {
public:
	inline Model() { m_Bound = false; };
	~Model();

	Mesh* m_Mesh = nullptr;
	VertexBuffer *m_Vb = nullptr;
	IndexBuffer *m_Ib = nullptr;
	VertexArray *m_Va = nullptr;
	VertexBufferLayout *m_Layout = nullptr;

	bool m_Active;
	bool m_Bound = false;

	void addVB();
	void addIB();
	void addVA();
	void addLayout();
	void Bind();
	void Set(Mesh* mesh, bool active);

	bool isValid();
};