#pragma once

#include "Test.h"
#include "Texture.h"
#include "Model.h"
#include "ChunkManager.h"
#include "Skybox.h"

namespace test {
	class TestBatching : public Test {
	public:
		TestBatching();
		~TestBatching();

		void OnUpdate(double deltaTime) override;
		void OnRender(int screenWidth, int screenHeight);
		void OnImGuiRender() override;

	private:
		Shader m_Shader;
		Texture m_Texture;
		CubeMap m_CubeMap;
		CubeMap m_Overlay;
		Skybox m_Skybox;

		Renderer& m_Renderer = Renderer::Get();

		glm::mat4 m_Proj;
		glm::mat4 m_ProjCam;
		glm::mat4 m_Model;
		glm::mat4 m_Mvp;

		glm::vec3 m_Rotation = glm::vec3(0.0f);

		float m_Color[4] = { 0.65f,0.85f,0.35f,1 };

		void MouseMovement();
		void KeyboardMovement(double deltaTime);
		void ChangeCursorLockState();
		void ExitApplication();

	};
}