#include "Renderer.h"

Renderer Renderer::s_Instance;

Renderer::Renderer()
{
}

void Renderer::Init() {
	m_LineShader = new Shader("res/shaders/line.shader");
	if(m_Line != nullptr) m_Line = new Line;
}

void Renderer::Draw(VertexArray& va, const IndexBuffer& ib) const
{
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::AddLine(Vector3::Float start, Vector3::Float end)
{
	if (m_Line == nullptr) m_Line = new Line;
	m_Line->Add(start, end);
}

void Renderer::DrawLines(glm::mat4& mat)
{
	if (m_LineShader == nullptr || m_Line == nullptr) return;

	m_Line->UpdateBuffer();

	m_LineShader->Bind();
	m_LineShader->SetUniform3f("u_Color", glm::vec3(0, 1, 0));
	m_LineShader->SetUniformMat4f("u_MVP", mat);

	m_Line->m_Va->Bind();

	glDrawArrays(GL_LINES, 0, m_Line->m_Vertices.size());
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
