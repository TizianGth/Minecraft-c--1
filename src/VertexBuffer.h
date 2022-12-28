#pragma once

#include <vector>

class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	// Before:	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void Set(const void *data, unsigned int size) const;
	void Add(const void* data, unsigned int size) const;
};
