#pragma once

#include <vector>
#include <glad/gl.h>


struct VertexBufferELement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		return 0;
	}

	VertexBufferELement(unsigned int _type, unsigned int _count, unsigned char normalized) {
		type = _type;
		count = _count;
		normalized = normalized;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferELement> m_Element;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {}

	template<typename t>
	void Push(unsigned int count) {
		//static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {

		m_Element.push_back(VertexBufferELement(GL_FLOAT, count, GL_FALSE ));
		m_Stride += VertexBufferELement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {

		m_Element.push_back(VertexBufferELement(GL_UNSIGNED_INT, count, GL_FALSE));
		m_Stride += VertexBufferELement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {

		m_Element.push_back(VertexBufferELement(GL_UNSIGNED_BYTE, count, GL_TRUE));
		m_Stride += VertexBufferELement::GetSizeOfType(GL_UNSIGNED_BYTE * count);
	}

	inline const std::vector<VertexBufferELement> GetElements() const& { return m_Element; }
	inline unsigned int GetStride() const { return m_Stride; }
};