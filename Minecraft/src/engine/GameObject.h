#pragma once
#include "../physics/BoxCollider.h"


class GameObject {
public:
	glm::vec3 m_Position = glm::vec3(0.0f,0.0f,0);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 m_Direction = glm::normalize(glm::vec3() - glm::vec3(0.0f, 0, 1.0f));
	glm::vec3 m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Direction));
	glm::vec3 m_Up = glm::cross(m_Direction, m_Right);

	bool m_IsCollider = false;

	BoxCollider m_BoxCollider;

	inline virtual void Rotate(const glm::vec3& rotation) {
		m_Direction.x = sin(glm::radians(-rotation.x)) * cos(glm::radians(rotation.y));
		m_Direction.y = -sin(glm::radians(rotation.y));
		m_Direction.z = cos(glm::radians(-rotation.x)) * cos(glm::radians(rotation.y));

		m_Direction = glm::normalize(m_Direction);
		m_Right = glm::normalize(glm::cross(m_Direction, m_Up));
		m_rotation = rotation;
		UpdateMat4();
	}
	inline virtual void MoveBy(const glm::vec3& velocity) {

		m_Position += velocity;
		m_BoxCollider.m_Position = m_Position;
		UpdateMat4();
	}
	inline virtual void MoveTo(const glm::vec3& position) {
		m_Position = position;
		UpdateMat4();
	}
	inline void SetColliderSize(const glm::vec3& size) {
		m_BoxCollider.m_Position = size;
		m_BoxCollider.m_Position = m_Position;
	}

	inline virtual void UpdateMat4() {}

	inline virtual void UpdateCollider(const glm::vec3& velocity) {
		m_BoxCollider.m_Velocity = velocity;

	}
};