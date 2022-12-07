#include "TestBatching.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"
#include "Input.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include "Application.h"
#include <glm/gtx/euler_angles.hpp>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <algorithm>
#include <chrono>

Textures grassOverlay[6] = { GrassMask,GrassMask,Grass, Null, GrassMask,GrassMask };
Textures null[6] = { Null,Null, Null, Null, Null, Null };
Textures DirtBlock[6] = { Dirt,Dirt,Dirt, Dirt, Dirt,Dirt };

Chunk* chunk = nullptr;

int SCREEN_WIDTH = 640 * 2;
int SCREEN_HEIGHT = 480 * 2;
float screenHalfX = SCREEN_WIDTH / 2;
float screenHalfY = SCREEN_HEIGHT / 2;
GLFWwindow* applWindow = nullptr;
test::TestBatching::TestBatching() : m_Shader("res/shaders/grass.shader"), m_Texture("res/textures/dirt.png")
{
	m_CubeMap.Load(DirtBlock, 0);
	m_Overlay.Load(grassOverlay, 0);

	chunk = new Chunk;
	chunk->Generate();
	chunk->FillUpTest();
	chunk->GenerateMeshes();

	m_Proj = glm::perspective(
		glm::radians(70.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		0.5f, 1000.0f);
	m_View = glm::mat4(1.0f);
	m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 00.0f));
	m_Model = glm::mat4(1.0f);

	m_Shader.Bind();
	m_CubeMap.Bind();
	m_Shader.SetUniform1i("u_Texture", 0);

	m_Overlay.Bind(1);
	m_Shader.SetUniform1i("u_Overlay", 1);
	m_Shader.Bind();

	applWindow = Application::GetWindow();
	glfwSetCursorPos(applWindow, screenHalfX, screenHalfY);
}

test::TestBatching::~TestBatching()
{
}

bool cursorLocked = true;
void test::TestBatching::OnUpdate(float deltaTime)
{
	if (applWindow == nullptr) {
		applWindow = Application::GetWindow();
	}
	MouseMovement();
	KeyboardMovement(deltaTime);
	ChangeCursorLockState();
}
void test::TestBatching::OnRender(int screenWidth, int screenHeight)
{
	m_Renderer.Clear();

	/*
	1. Make 1 Big VA, BV, etc for each chunk and only change when needed
	2. Change VB, Va on the fly with eg Vectors to only bind visible/used information for each chunk (every frame?)
	
	*/

	m_Shader.Bind();

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


	// rendering 14x14 chunks atm all use the same Mesh/Model, but already seperate draw calls
	// seperate draw calls prop better becasue otherwise the buffers and everything woudl just be 1 giant blob + 14*14 draw calls isnt the worst
	for (int chunkInstanceX = 0; chunkInstanceX < 14; chunkInstanceX++) {
		for (int chunkInstanceZ = 0; chunkInstanceZ < 14; chunkInstanceZ++) {
			{
				m_Model = glm::translate(glm::mat4(1.0f), translation + glm::vec3(chunkInstanceX * CHUNK_SIZE, 0.0f, chunkInstanceZ * CHUNK_SIZE));
				// model = glm::scale(model, glm::vec3(1f, 0.1f, 0.1f));
				/*m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));;
				m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));;
				m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));; */
				glm::mat4 mvp = m_Proj * m_View * m_Model;

				m_Shader.SetUniformMat4f("u_MVP", mvp);


				m_Renderer.Draw(chunk->m_Model.m_Va, chunk->m_Model.m_Ib, m_Shader);

			}
		}
	}
}

void test::TestBatching::OnImGuiRender()
{
	ImGui::SliderFloat("Translation X", &m_TranslationX, -20, 20);
	ImGui::SliderFloat("Translation Y", &m_TranslationY, -20, 20);
	ImGui::SliderFloat("Translation Z", &m_TranslationZ, -100.0f, 0);
	ImGui::ColorEdit4("Color", m_Color);

	//ImGui::SliderFloat3("Rotation", &m_Rotation.x, -360, 360);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}


double rotationX = -90;
double rotationY = 0;

double rotationSpeed = 0.3f;
float movementSpeed = 0.01f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

glm::vec3 direction;
glm::vec3 cameraFront;
void test::TestBatching::MouseMovement()
{
	if (!cursorLocked) return;
	auto position = Input::GetMousePosition();
	glfwSetCursorPos(applWindow, screenHalfX, screenHalfY);
	//std::cout << position.first - screenHalfX << " " << position.second - screenHalfY << std::endl;

	glm::vec2 delta = { position.first - screenHalfX,  position.second - screenHalfY };

	// numbers were getting to big for float/double to hold, so I reset everytime bigger than 360 
	// this is not optimal but most of the time nobody will see the reset 
	if (rotationX >= 360 || rotationX <= -360) {
		rotationX = 0;
		//std::cout << "Reset" << std::endl;
	}
	rotationX += delta.x * rotationSpeed;
	
	if (rotationY >= 360 || rotationY <= -360) {
		rotationY = 0;
		//std::cout << "Reset" << std::endl;
	}
	rotationY -= delta.y * rotationSpeed;
	rotationY = std::clamp(rotationY, -89.9, 89.9);
	



	//std::cout << "X: " << rotationX << std::endl;
	//std::cout << "Y: " << rotationY << std::endl;
	direction.x = cos(glm::radians(rotationX)) * cos(glm::radians(rotationY));
	direction.y = sin(glm::radians(rotationY));
	direction.z = sin(glm::radians(rotationX)) * cos(glm::radians(rotationY));
	
	cameraFront = glm::normalize(direction);
	m_View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
void test::TestBatching::KeyboardMovement(float deltaTime)
{
	if (Input::IsKeyPressed(GLFW_KEY_W)) {
		cameraPos += cameraFront * movementSpeed * deltaTime;
	}
	if (Input::IsKeyPressed(GLFW_KEY_S)) {
		cameraPos -= cameraFront * movementSpeed * deltaTime;
	}
	if (Input::IsKeyPressed(GLFW_KEY_A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * movementSpeed * deltaTime;
	}
	if (Input::IsKeyPressed(GLFW_KEY_D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * movementSpeed * deltaTime;
	}
}

void test::TestBatching::ChangeCursorLockState()
{
	if (Input::IsKeyPressed(GLFW_KEY_L)) {
		cursorLocked = !cursorLocked;
	}

	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwDestroyWindow(Application::GetWindow());
	}
}
