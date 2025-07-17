#pragma once

#include <iostream>
#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Line.h"

class Renderer {
public:
	static Renderer& Get() { return s_Instance; };

	~Renderer() = default;

	void Init();

	void Draw(const unsigned int va, const unsigned int indicesCount) const;

	void Clear() const;

	glm::vec3 m_ClearColor = glm::vec3(0.8f, 1.0f, 1.0f);

private:
	Renderer();

	static Renderer s_Instance;
};
