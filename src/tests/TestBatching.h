#pragma once

#include "Test.h"
#include "Texture.h"
#include "Model.h"
#include "Chunk.h"

namespace test {
	class TestBatching : public Test {
	public:
		TestBatching();
		~TestBatching();

		void OnUpdate(float deltaTime) override;
		void OnRender(glm::vec3 translation, glm::vec3 roation, float color[4]);
		void OnImGuiRender() override;

	private:
		//float vertecies[48];
		//unsigned int indices[12];

		//VertexArray m_Va;
		//VertexBuffer m_Vb;
		//IndexBuffer m_Ib;
		VertexBufferLayout m_Layout;

		//Renderer m_Renderer;
		Shader m_Shader;
		Texture m_Texture;
		CubeMap m_CubeMap;
		CubeMap m_Overlay;
		Renderer m_Renderer;
		Chunk m_Chunck;


		glm::mat4 proj;
		glm::mat4 view;

	};
}