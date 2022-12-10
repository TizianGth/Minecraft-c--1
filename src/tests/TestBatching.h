#pragma once

#include "Test.h"
#include "Texture.h"
#include "Model.h"
#include "Chunk.h"
#include "Skybox.h"

namespace test {
	class TestBatching : public Test {
	public:
		TestBatching();
		~TestBatching();

		void OnUpdate(float deltaTime) override;
		void OnRender(int screenWidth, int screenHeight);
		void OnImGuiRender() override;

	private:

		Shader m_Shader;
		Texture m_Texture;
		//CubeMap m_CubeMaps[]
		CubeMap m_CubeMap;
		CubeMap m_Overlay;
		Renderer m_Renderer;
		Skybox m_Skybox;
		Chunk* m_Chunks[14][14];


		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;


		// imgui
		float m_TranslationX = 0;
		float m_TranslationY = -2;
		float m_TranslationZ = -6;

		glm::vec3 m_Rotation = glm::vec3(0.0f);

		float m_Color[4] = { 0.65f,0.85f,0.35f,1 };

		void MouseMovement();
		void KeyboardMovement(float deltaTime);
		void ChangeCursorLockState();
	};
}