#include "VertexBuffer.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    if (this == nullptr) return;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Set(const void* data, unsigned int size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Add(const void* data, unsigned int size) const
{
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

