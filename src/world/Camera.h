#pragma once

#include "GameObject.h"

class Camera : public GameObject {
public:
	glm::mat4 m_Mat4 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	float m_RotationSpeed = 1;
	float m_MovementSpeed = 0.012f;


	BoxCollider boxCollider = BoxCollider(m_Position - glm::vec3(0, 0.7f, 0), glm::vec3(0.8f, 2.0f, 8.5f));

	inline void UpdateCollider(glm::vec3 position) override {
		boxCollider.Update(position - glm::vec3(0,0.7f, 0), glm::vec3(0.8f, 2.0f, 0.8f));
	}
	// moved mat4 here so not every gameobj has a mat4
	inline void UpdateMat4() override {
		m_Mat4 = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	}

};