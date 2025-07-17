#pragma once

#include "GameObject.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	Camera() {};

	inline Camera(int width, int height)
	{ 
		SetFov(m_Fov, width, height); 

		m_Collider.m_Position = m_Position - glm::vec3(0.5f, m_CameraHeight, 0.5f);
		m_Collider.m_Size = glm::vec3(1.0f, m_ColliderHeight, 1.0f);

		m_IsCollider = true;
	};

	inline void SetFov(unsigned int fov, int windowWith, int windowHeight) {
		m_Fov = fov;
		m_AspectRatio = (float)windowWith / (float)windowHeight;
		m_Proj = glm::perspective(glm::radians((float)fov), m_AspectRatio, m_NearPlane, m_FarPlane);

	};

	// moved mat4 here so not every gameobj has a mat4
	inline void UpdateMat4() override {
		m_Mat4 = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	}

	inline void MoveByCollider(glm::vec3 velocity, BoxCollider& collider) {

	}

};