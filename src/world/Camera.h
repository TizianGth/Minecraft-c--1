#pragma once

#include "GameObject.h"

class Camera : public GameObject {
public:
	glm::mat4 m_Mat4 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	float m_RotationSpeed = 1;
	float m_MovementSpeed = 10000;

	// moved mat4 here so not every gameobj has a mat4
	inline void UpdateMat4() override {
		m_Mat4 = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	}
};