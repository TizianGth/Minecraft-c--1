#include "TestBatching.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"

Textures GrassBlock[6] = { Dirt,Dirt,Grass, Dirt, Dirt,Dirt };
Textures blockOverlay[6] = { GrassMask,GrassMask,Grass, Null, GrassMask,GrassMask };

Chunk* chunk = nullptr;

int SCREEN_WIDTH = 640 * 2;
int SCREEN_HEIGHT = 480 * 2;

test::TestBatching::TestBatching() : m_Shader("res/shaders/grass.shader"), m_Texture("res/textures/dirt.png"), m_CubeMap(GrassBlock, 0), m_Overlay(blockOverlay, 0)
{


	/*struct Vertex {
		glm::vec3 Position[3];
		//float Color[4];
		glm::vec2 TexCoords[3];
		float TexID;
	}; */

	chunk = new Chunk;
	chunk->Generate();
	chunk->FillUpTest(1);
	chunk->GenerateMeshes();

	m_Proj = glm::perspective(
		glm::radians(70.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		0.5f, 1000.0f);
	m_View = glm::mat4(1.0f);
	m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -1.0f));
	m_Model = glm::mat4(1.0f);

	m_Shader.Bind();
	m_CubeMap.Bind();
	m_Shader.SetUniform1i("u_Texture", 0);

	m_Overlay.Bind(1);
	m_Shader.SetUniform1i("u_Overlay", 1);
	m_Shader.Bind();
}

test::TestBatching::~TestBatching()
{
}

void test::TestBatching::OnUpdate(float deltaTime)
{
}
void test::TestBatching::OnRender(int screenWidth, int screenHeight)
{
	m_Renderer.Clear();

	m_Shader.Bind();
	{

		/*
		1. Make 1 Big VA, BV, etc for each chunk and only change when needed
		2. Change VB, Va on the fly with eg Vectors to only bind visible/used information for each chunk (every frame?)
		
		*/

		m_Overlay.Bind(1);
		m_Shader.SetUniform1i("u_Overlay", 1);
		m_CubeMap.Bind(0);
		m_Shader.SetUniform1i("u_Texture", 0);

		m_Proj = glm::perspective(
			glm::radians(70.0f),
			(float)screenWidth / (float)screenHeight,
			0.5f, 1000.0f);
		glm::vec3 translation = glm::vec3(m_TranslationX, m_TranslationY, m_TranslationZ);
		m_Shader.SetUniform4f("u_Color", glm::vec4(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		m_Model = glm::translate(glm::mat4(1.0f), translation - glm::vec3(1.0f, 1.0f, 1.0f));
		// model = glm::scale(model, glm::vec3(1f, 0.1f, 0.1f));
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));;
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));;
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));;
		glm::mat4 mvp = m_Proj * m_View * m_Model;

		m_Shader.SetUniformMat4f("u_MVP", mvp); 

		m_Renderer.Draw(chunk->m_Model.m_Va, chunk->m_Model.m_Ib, m_Shader);
	}


}

void test::TestBatching::OnImGuiRender()
{
	ImGui::SliderFloat("Translation X", &m_TranslationX, -20, 20);
	ImGui::SliderFloat("Translation Y", &m_TranslationY, -20, 20);
	ImGui::SliderFloat("Translation Z", &m_TranslationZ, -100.0f, 0);
	ImGui::ColorEdit4("Color", m_Color);

	ImGui::SliderFloat3("Rotation", &m_Rotation.x, -360, 360);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}