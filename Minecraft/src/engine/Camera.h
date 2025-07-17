#pragma once

#include "GameObject.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Input.h"
#include <algorithm>
#include "DeltaTime.h"
#include "AABB.h"

class Camera : public GameObject {
public:
	BoxCollider m_Collider;

	glm::mat4 m_Mat4 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 m_Proj = glm::mat4(1.0f);

	float m_RotationSpeed = 0.03f;
	float m_MovementSpeed = 0.012f;
	float m_MovementSpeedMaxMulti = 8;
	float m_ColliderHeight = 2.0f;
	float m_CameraHeight = 1.5f;

	float m_NearPlane = 0.2f;
	float m_FarPlane = 2000.0f;

	unsigned int m_Fov = 90.0f;
	float m_AspectRatio = 0.0f;

	bool m_MouseLocked = true;

	Camera() {};

	Camera(int width, int height, GLFWwindow* window);
	void SetFov(unsigned int fov);

	// moved mat4 here so not every gameobj has a mat4
	void UpdateMat4() override;

	void MoveByCollider(glm::vec3 velocity, BoxCollider& collider);

	void OnMouseMove();
	void OnKeyboardMove();
	void OnZoom();
	void ZoomIn();
	void ZoomOut();
	void OnMouseLock();

	void SetWindow(GLFWwindow* window);

	// TODO: change to box collider
	bool IsInFrustum(const AABB& aabb);
	void SetPlanes(const glm::mat4& m);
private:
	int m_WindowWidth = 0, m_WindowHeight = 0;
	double m_RotationX = 0, m_RotationY = 0;
	double m_LastX = 0, m_LastY = 0;
	GLFWwindow* m_Window = nullptr;

	glm::vec4 m_Planes[6];
};