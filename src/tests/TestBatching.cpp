#include "TestBatching.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"
#include "vector.h"

Textures GrassBlock[6] = { Dirt,Dirt,Grass, Dirt, Dirt,Dirt };
Textures blockOverlay[6] = { GrassMask,GrassMask,Grass, Null, GrassMask,GrassMask };
vector3 positions[ChunkSize] = { vector3(0,0,0),  vector3(2,0,0),  vector3(-2,0,0),
								 vector3(0,0,2),  vector3(2,0,2),  vector3(-2,0,2),
								 vector3(0,0,-2), vector3(2,0,-2), vector3(-2,0,-2) };

test::TestBatching::TestBatching() : m_Shader("res/shaders/grass.shader"), m_Texture("res/textures/dirt.png"), m_CubeMap(GrassBlock, 0), m_Overlay(blockOverlay, 0),
m_Chunck(positions)
{


	std::vector<float> vertices = {
	   -1, -1, 1, -1, -1, 1, 0,
		1, -1, 1,  1, -1, 1, 0,
		1,  1, 1,  1,  1, 1, 0,
	   -1,  1, 1, -1,  1, 1, 0,

	   -1, -1, -1, -1, -1, -1, 0,
		1, -1, -1,  1, -1, -1, 0,
		1,  1, -1,  1,  1, -1, 0,
	   -1,  1, -1, -1,  1, -1, 0
	};

	std::vector<int> indices = {
		// Front
		 0,1,2,
		 2,3,0,

		 // Back
		 4,5,6,
		 6,7,4,

		 // Right
		 1, 5, 6,
		 6, 2, 1,

		 // Left
		 0, 4, 7,
		 7, 3, 0,

		 // Top
		 2, 3, 7,
		 7, 2, 6,

		 // Bottom
		 1, 5, 4,
		 4, 1, 0
	};

	struct Vertex {
		glm::vec3 Position[3];
		//float Color[4];
		glm::vec2 TexCoords[3];
		float TexID;
	};


	/*Mesh cube;
	cube.vertices = vertices;
	cube.indices = indices;

	m.Set(cube);
	m.addVB();
	m.addIB();
	m.addVA(); */

	m_Chunck.Set();

	proj = glm::perspective(
		glm::radians(90.0f),
		float(640 * 2) / float(480 * 2),
		0.1f, 100.0f);
	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
	m_Shader.Bind();
	// Bind(0) => 0 => SetUniform("texture", 0)
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
void test::TestBatching::OnRender(glm::vec3 translation, glm::vec3 roation, float color[4])
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
		m_Shader.SetUniform4f("u_Color", vector4(color[0], color[1], color[2], color[3]));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		// model = glm::scale(model, glm::vec3(1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(roation.x), glm::vec3(0.0f, 1.0f, 0.0f));;
		model = glm::rotate(model, glm::radians(roation.y), glm::vec3(1.0f, 0.0f, 0.0f));;
		model = glm::rotate(model, glm::radians(roation.z), glm::vec3(0.0f, 0.0f, 1.0f));;
		glm::mat4 mvp = proj * view * model;

		m_Shader.SetUniformMat4f("u_MVP", mvp);

		m_Renderer.Draw(m_Chunck.m_Models[0].m_Va, m_Chunck.m_Models[0].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[1].m_Va, m_Chunck.m_Models[1].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[2].m_Va, m_Chunck.m_Models[2].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[3].m_Va, m_Chunck.m_Models[3].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[4].m_Va, m_Chunck.m_Models[4].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[5].m_Va, m_Chunck.m_Models[5].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[6].m_Va, m_Chunck.m_Models[6].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[7].m_Va, m_Chunck.m_Models[7].m_Ib, m_Shader);
		m_Renderer.Draw(m_Chunck.m_Models[8].m_Va, m_Chunck.m_Models[8].m_Ib, m_Shader);
	}


}

void test::TestBatching::OnImGuiRender()
{
}