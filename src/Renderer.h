#pragma once

#include <iostream>
#include <glad/gl.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader.h"
#include "Line.h"


class Renderer {
public:
	static Renderer& Get() { return s_Instance; };

	~Renderer() = default;

	void Init();

	void Draw(VertexArray& va, const IndexBuffer& ib) const;
	void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void AddLine(Vector3::Float start, Vector3::Float end);
	void DrawLines(glm::mat4& mat);

	void Clear() const;

private:
	Renderer();

	static Renderer s_Instance;

	Shader* m_LineShader = nullptr;
	Line* m_Line = nullptr;
};
