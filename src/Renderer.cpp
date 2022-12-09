#include "Renderer.h"

void Renderer::Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    //shader.Bind();
    va.Bind();
    ib.Bind(); 

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
} 

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
