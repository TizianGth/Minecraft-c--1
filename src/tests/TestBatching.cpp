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

BoxCollider block(glm::vec3(0, 5, 0), glm::vec3(10, 10, 10));
BoxCollider block2(glm::vec3(11, 5, 0), glm::vec3(10, 10, 10));
test::TestBatching::TestBatching() : m_Shader("res/shaders/grass.shader"), m_Texture("res/textures/dirt.png")
{
	m_Renderer.Init();
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
	cam.m_Position += glm::vec3(0, 30, 0);
	cam.m_RotationSpeed = 0.3f;

	ChunkManager::Get().GenerateChunks();
	ChunkManager::Get().m_ActivLastChunk = Vector2::Int((std::floor(cam.m_Position.x / 16) + 2), (std::floor(cam.m_Position.z / 16) + 2));

	block.SetDisplay();
	block2.SetDisplay();
}

test::TestBatching::~TestBatching()
{
}

bool cursorLocked = true;
void test::TestBatching::OnUpdate(double deltaTime)
{
	if (applWindow == nullptr) {
		applWindow = Application::GetWindow();
	}
	MouseMovement();
	KeyboardMovement(deltaTime);

	ChangeCursorLockState();
	ExitApplication();

	ChunkManager& cm = ChunkManager::Get();
	cm.UpdateChunks(Vector2::Int((std::floor(cam.m_Position.x / 16) + 2), (std::floor(cam.m_Position.z / 16) + 2)));

	ThreadManager::Get().UpdateOnMainThread();

	/*for (int x = -0; x < 1; x++) {
		for (int z = 0; z < 1; z++) {
			for (int y = -0; y < 2; y++) {
				Chunk* chunk = cm.GetChunksPointer()->at(0).at(0).get();
				if (chunk != nullptr) {
					std::cout << chunk->GetBlock(glm::vec3(cam.m_Position.x - chunk->m_ChunkWorldPosition.x + x, cam.m_Position.y + y, cam.m_Position.z - chunk->m_ChunkWorldPosition.y + z)) << std::endl;
				}
			}
		}
	} */



	//ChunkManager::Get().GetChunksPointer()->at(0).at(0).get().
}
double rotationX = -90;
double rotationY = -10;

float fogOpacity = 1;
float fogColor[3] = { 0.639f,0.776f,1 };


void test::TestBatching::OnRender(int screenWidth, int screenHeight)
{
	m_Renderer.Clear();

	std::vector<std::vector<std::shared_ptr<Chunk>>>& chunks = *ChunkManager::Get().GetChunksPointer();

	/*
	1. Make 1 Big VA, BV, etc for each chunk and only change when needed
	2. Change VB, Va on the fly with eg Vectors to only bind visible/used information for each chunk (every frame?)
	*/

	m_Proj = glm::perspective(
		glm::radians(70.0f),
		(float)screenWidth / (float)screenHeight,
		0.2f, 1000.0f);

	m_ProjCam = m_Proj * cam.m_Mat4;

	m_Model = glm::translate(glm::mat4(1.0f), cam.m_Position);
	//m_Model = glm::scale(m_Model, glm::vec3(12.43796f, 12.43796f, 12.43796f)); Would need to do this when using short
	m_Mvp = m_ProjCam * m_Model;

	m_Skybox.Render(m_Mvp);
	m_Renderer.Draw(*m_Skybox.m_Model.m_Va, *m_Skybox.m_Model.m_Ib);

	m_Model = glm::mat4(1.0f);
	m_Mvp = m_ProjCam * m_Model;

	m_Renderer.DrawLines(m_Mvp);

	m_Shader.Bind();
	m_Shader.SetUniform4f("u_Color", glm::vec4(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
	m_Shader.SetUniform1f("u_FogOpacity", fogOpacity);
	m_Shader.SetUniform4f("u_FogColor", glm::vec4(fogColor[0], fogColor[1], fogColor[2], 1));
	int dimensions = ChunkManager::Get().GetDimensions();

	//std::cout << cam.boxCollider1.Collision(cam.boxCollider2) << std::endl;

	// rendering 14x14 chunks atm all use the same Mesh/Model, but already seperate draw calls
	// seperate draw calls prop better becasue otherwise the buffers and everything woudl just be 1 giant blob + 14*14 draw calls isnt the worst

	/*std::cout << cam.m_Direction.x << std::endl;
	std::cout << cam.m_rotation.x << std::endl << std::endl;
	*/

	//m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(1,1,1));
	//m_Model = glm::scale(m_Model, glm::vec3(12.43796f, 12.43796f, 12.43796f)); Would need to do this when using short
	//m_Mvp = m_ProjCam * m_Model;


	for (int chunkInstanceX = 0; chunkInstanceX < dimensions; chunkInstanceX++) {
		for (int chunkInstanceZ = 0; chunkInstanceZ < dimensions; chunkInstanceZ++) {
			{
				if (chunks.at(chunkInstanceX).at(chunkInstanceZ) != nullptr) {
					auto chunk = chunks.at(chunkInstanceX).at(chunkInstanceZ);
					if (chunk->m_Render && chunk->m_Model.m_Bound) {
						// TODO: save model matrix in chunk class
						m_Model = glm::translate(glm::mat4(1.0f), glm::vec3((chunk->m_ChunkWorldPosition.x - dimensions / 2) * CHUNK_SIZE, 0.0f, (chunk->m_ChunkWorldPosition.y - dimensions / 2) * CHUNK_SIZE));
						m_Mvp = m_ProjCam * m_Model;

						m_Shader.SetUniformMat4f("u_MVP", m_Mvp);
						m_Renderer.Draw(*chunk->m_Model.m_Va, *chunk->m_Model.m_Ib);
					}
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

	ImGui::SliderFloat3("Fog Color", fogColor, 0.0f, 1.0f);
	ImGui::SliderFloat("Toggle Fog", &fogOpacity, 0.0f, 1.0f);
}

int screenHalfX;
int screenHalfY;
void test::TestBatching::MouseMovement()
{
	if (!cursorLocked) return;
	auto position = Input::GetMousePosition();
	glfwSetCursorPos(applWindow, screenHalfX, screenHalfY);
	screenHalfX = Application::GetWindowWidth() / 2;
	screenHalfY = Application::GetWindowHeight() / 2;

	glm::vec2 delta = { position.x - screenHalfX, position.y - screenHalfY };



	rotationX += delta.x * cam.m_RotationSpeed;
	rotationY += delta.y * cam.m_RotationSpeed;
	rotationY = std::clamp(rotationY, -89.9, 89.9);

	cam.Rotate(glm::vec3(rotationX, rotationY, 0));

}

void test::TestBatching::KeyboardMovement(double deltaTime)
{
	if (!cursorLocked) return;

	glm::vec3 pos = cam.m_Position;	
	std::vector<BoxCollider> colls = { block, block2 };

	if (Input::IsKeyPressed(GLFW_KEY_W)) { // Todo: maybe if velocity >= eg 0.5 check if every 0.5 position colliding then go back and reduce 0.5 to smaller like 0.1 

		glm::vec3 vel = glm::vec3(cam.m_MovementSpeed * deltaTime * cam.m_Direction.x, cam.m_MovementSpeed * deltaTime * cam.m_Direction.y, cam.m_MovementSpeed * deltaTime * cam.m_Direction.z);
		auto collision = cam.boxCollider.Colliding(colls);

			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				if (collision.size() > 0) {
					cam.MoveBy(vel * cam.boxCollider.GetCollidingNormals(collision, vel));
				}
				else {
					glm::vec3 pos = cam.m_Position;
					cam.MoveBy(vel);
				}
			}
			else {
				if (collision.size() > 0) {
					cam.MoveBy(vel * cam.boxCollider.GetCollidingNormals(collision, vel) * glm::vec3(0.1f,0.1f,0.1f));
				}
				else {
					cam.MoveBy(vel * glm::vec3(0.1f, 0.1f, 0.1f));
				}
			}
	}
	if (Input::IsKeyPressed(GLFW_KEY_S)) {
		glm::vec3 vel = glm::vec3(-cam.m_MovementSpeed * deltaTime * cam.m_Direction.x, -cam.m_MovementSpeed * deltaTime * cam.m_Direction.y, -cam.m_MovementSpeed * deltaTime * cam.m_Direction.z);
		auto collision = cam.boxCollider.Colliding(colls);
		if (collision.size() > 0) {
			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				cam.MoveBy(vel * cam.boxCollider.GetCollidingNormals(collision, vel));
			}
		}
		else {
			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				cam.MoveBy(vel);
			}
		}
	}
	if (Input::IsKeyPressed(GLFW_KEY_A)) {
		glm::vec3 vel = glm::vec3(-cam.m_MovementSpeed * deltaTime * cam.m_Right.x, -cam.m_MovementSpeed * deltaTime * cam.m_Right.y, -cam.m_MovementSpeed * deltaTime * cam.m_Right.z);
		auto collision = cam.boxCollider.Colliding(colls);
		if (collision.size() > 0) {
			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				cam.MoveBy(vel * cam.boxCollider.GetCollidingNormals(collision, vel));
			}
		}
		else {
			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				cam.MoveBy(vel);
			}
		}
	}
	if (Input::IsKeyPressed(GLFW_KEY_D)) {
		glm::vec3 vel = glm::vec3(cam.m_MovementSpeed * deltaTime * cam.m_Right.x, cam.m_MovementSpeed * deltaTime * cam.m_Right.y, cam.m_MovementSpeed * deltaTime * cam.m_Right.z);
		auto collision = cam.boxCollider.Colliding(colls);
		if (collision.size() > 0) {
			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				cam.MoveBy(vel * cam.boxCollider.GetCollidingNormals(collision, vel));
			}
		}
		else {
			if (!(std::abs(vel.x) >= 0.1f || std::abs(vel.y) >= 0.1f || std::abs(vel.z) >= 0.1f)) {
				cam.MoveBy(vel);
			}
		}
	}
}

void test::TestBatching::ChangeCursorLockState()
{
	if (Input::IsKeyPressed(GLFW_KEY_L)) {
		cursorLocked = !cursorLocked;
		cam.boxCollider.SetDisplay();
	}
}

void test::TestBatching::ExitApplication()
{
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(applWindow, GLFW_TRUE);
	}
}


