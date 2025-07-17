#pragma once

#include <vector>
#include <glad/glad.h>

class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer();
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void Set(const void *data, unsigned int size) const;
	void Replace(const void* data, unsigned int size) const;
};
