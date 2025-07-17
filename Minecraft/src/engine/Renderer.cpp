#include "Renderer.h"
#include <chrono>

Renderer Renderer::s_Instance;

Renderer::Renderer()
{
}

void Renderer::Init() {
}

void Renderer::Draw(const unsigned int va, const unsigned int indicesCount) const
{

	glBindVertexArray(va);

	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);

}

void Renderer::Clear() const {
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, 1.0f);
}
