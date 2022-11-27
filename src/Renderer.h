#pragma once

#include <glad/gl.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader.h"


class Renderer {
public:
	void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
};
