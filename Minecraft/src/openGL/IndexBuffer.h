#pragma once


#include <glad/glad.h>

class IndexBuffer {
private:
	unsigned int m_RendererID;
	int m_Count = 0;
public:

	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Set(const void* data, unsigned int count);

	inline unsigned int GetCount() const { return m_Count; }
};
