#pragma once

#include "vector.h"
#include "VertexArray.h"
#include <unordered_map>
#include <iostream>

class Line {
public:
	std::vector<Vector3::Float> m_Vertices;


	bool changed = false;
	inline Line() {
		m_Layout = new VertexBufferLayout;
		m_Vb = new VertexBuffer;
		m_Va = new VertexArray;
		m_Layout->Push<float>(3);
	};

	inline void Add(Vector3::Float start, Vector3::Float end) {
		m_Vertices.push_back(start);
		m_Vertices.push_back(end);

		changed = true;
	}

	inline void UpdateBuffer() {
		if (!changed) return;

		delete m_Vb;
		delete m_Va;

		m_Vb = new VertexBuffer;
		m_Va = new VertexArray;

		std::vector<float> verts;

		int size = m_Vertices.size();
		for (int i = 0; i < size; i++) {




			verts.push_back(m_Vertices[i].x);
			verts.push_back(m_Vertices[i].y);
			verts.push_back(m_Vertices[i].z);
		}

		m_Vb->Set(verts.data(), sizeof(float) * verts.size());
		m_Va->AddBuffer(*m_Vb, *m_Layout);

		changed = false;
	}

	VertexBuffer* m_Vb = nullptr;
	VertexBufferLayout* m_Layout = nullptr;
	VertexArray* m_Va = nullptr;

	~Line() {
		delete m_Vb;
		delete m_Layout;
		delete m_Va;
	};
};