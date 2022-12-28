#include "TestBatching.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include "Application.h"
#include "ThreadManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include <algorithm>
#include <chrono>

Textures grassOverlay[6] = { GrassMask,GrassMask,Grass, Null, GrassMask,GrassMask };
Textures null[6] = { Null,Null, Null, Null, Null, Null };
Textures DirtBlock[6] = { Dirt,Dirt,Dirt, Dirt, Dirt,Dirt };

GLFWwindow* applWindow = nullptr;
Camera cam;
test::TestBatching::TestBatching() : m_Shader("res/shaders/grass.shader"), m_Texture("res/textures/dirt.png")
{

	m_CubeMap.Load(DirtBlock, 0);
	m_Overlay.Load(grassOverlay, 0);


	m_Proj = glm::perspective(
		glm::radians(70.0f),
		(float)Application::GetWindowWidth() / (float)Application::GetWindowHeight(),
		0.2f, 3000.0f);

	m_Model = glm::mat4(1.0f);

	m_Shader.Bind();
	m_CubeMap.Bind();
	m_Shader.SetUniform1i("u_Texture", 0);

	m_Overlay.Bind(1);
	m_Shader.SetUniform1i("u_Overlay", 1);
	m_Shader.Bind();

	applWindow = Application::GetWindow();
	glfwSetCursorPos(applWindow, Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2);
	cam.m_Position += glm::vec3(0, 10, 0);
	cam.m_RotationSpeed = 0.3f;
	cam.m_MovementSpeed = 0.01f;

	ChunkManager::Get().GenerateChunks();
	ChunkManager::Get().m_ActivLastChunk = Vector2::Int((std::floor(cam.m_Position.x / 16) + 2), (std::floor(cam.m_Position.z / 16) + 2));
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

	ChunkManager::Get().UpdateChunks(Vector2::Int((std::floor(cam.m_Position.x / 16) + 2), (std::floor(cam.m_Position.z / 16) + 2)));
	ThreadManager::Get().CheckForUpdates();
}
double rotationX = -90;
double rotationY = -10;

void test::TestBatching::OnRender(int screenWidth, int screenHeight)
{
	m_Renderer.Clear();

	auto chunks = ChunkManager::Get().GetChunksPointer();

	/*
	1. Make 1 Big VA, BV, etc for each chunk and only change when needed
	2. Change VB, Va on the fly with eg Vectors to only bind visible/used information for each chunk (every frame?)
	*/

	m_Proj = glm::perspective(
		glm::radians(70.0f),
		(float)screenWidth / (float)screenHeight,
		0.2f, 1000.0f);

	m_Model = glm::translate(glm::mat4(1.0f), cam.m_Position);
	//m_Model = glm::scale(m_Model, glm::vec3(12.43796f, 12.43796f, 12.43796f)); Would need to do this when using short
	m_Mvp = m_Proj * cam.m_Mat4 * m_Model;

	m_Skybox.Render(m_Mvp);
	m_Renderer.Draw(*m_Skybox.m_Model.m_Va, *m_Skybox.m_Model.m_Ib, m_Skybox.m_Shader);

	m_Shader.Bind();
	m_Shader.SetUniform4f("u_Color", glm::vec4(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
	int dimensions = ChunkManager::Get().GetDimensions();

	// rendering 14x14 chunks atm all use the same Mesh/Model, but already seperate draw calls
	// seperate draw calls prop better becasue otherwise the buffers and everything woudl just be 1 giant blob + 14*14 draw calls isnt the worst

	for (int chunkInstanceX = 0; chunkInstanceX < dimensions; chunkInstanceX++) {
		for (int chunkInstanceZ = 0; chunkInstanceZ < dimensions; chunkInstanceZ++) {
			{
				auto chunk = chunks[chunkInstanceX][chunkInstanceZ];
				if (chunk != nullptr && chunk->m_Model.isValid()) {
					m_Model = glm::translate(glm::mat4(1.0f), glm::vec3((chunk->m_ChunkWorldPosition.x - dimensions / 2) * CHUNK_SIZE, 0.0f, (chunk->m_ChunkWorldPosition.y - dimensions / 2) * CHUNK_SIZE));
					//m_Model = glm::scale(m_Model, glm::vec3(12.43796f, 12.43796f, 12.43796f));
					m_Mvp = m_Proj * cam.m_Mat4 * m_Model;

					m_Shader.SetUniformMat4f("u_MVP", m_Mvp);
					m_Renderer.Draw(*chunk->m_Model.m_Va, *chunk->m_Model.m_Ib, m_Shader);
				}
			}
		}
	}
}

void test::TestBatching::OnImGuiRender()
{
	ImGui::ColorEdit4("Color", m_Color);

	ImGui::Text("Player position (%.1f, %.1f, %.1f)", cam.m_Position.x, cam.m_Position.y, cam.m_Position.z);
	ImGui::Text("Player position in Chunk (%.1f, %.1f, %.1f)", (std::floor(cam.m_Position.x / 16) + 2), cam.m_Position.y, (std::floor(cam.m_Position.z / 16) + 2));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

int screenHalfX;
int screenHalfY;
void test::TestBatching::MouseMovement()
{
	if (!cursorLocked) return;
	auto position = Input::GetMousePosition();

	screenHalfX = Application::GetWindowWidth() / 2;
	screenHalfY = Application::GetWindowHeight() / 2;

	glfwSetCursorPos(applWindow, screenHalfX, screenHalfY);

	glm::vec2 delta = { position.first - screenHalfX,  position.second - screenHalfY };

	rotationX += delta.x * cam.m_RotationSpeed;
	rotationY -= delta.y * cam.m_RotationSpeed;
	rotationY = std::clamp(rotationY, -89.9, 89.9);

	cam.Rotate(glm::vec3(rotationX, rotationY, 0));

}
void test::TestBatching::KeyboardMovement(float deltaTime)
{
	if (!cursorLocked) return;

	if (Input::IsKeyPressed(GLFW_KEY_W)) {
		cam.m_Position += cam.m_Direction * cam.m_MovementSpeed * deltaTime;
	}
	if (Input::IsKeyPressed(GLFW_KEY_S)) {
		cam.m_Position -= cam.m_Direction * cam.m_MovementSpeed * deltaTime;
	}
	if (Input::IsKeyPressed(GLFW_KEY_A)) {
		cam.m_Position -= glm::normalize(glm::cross(cam.m_Direction, cam.m_Up)) * cam.m_MovementSpeed * deltaTime;
	}
	if (Input::IsKeyPressed(GLFW_KEY_D)) {
		cam.m_Position += glm::normalize(glm::cross(cam.m_Direction, cam.m_Up)) * cam.m_MovementSpeed * deltaTime;
	}
}

void test::TestBatching::ChangeCursorLockState()
{
	if (Input::IsKeyPressed(GLFW_KEY_L)) {
		cursorLocked = !cursorLocked;
	}

	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(Application::GetWindow(), GLFW_TRUE);
	}
}
