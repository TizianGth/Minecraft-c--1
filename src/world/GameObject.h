#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include "BoxCollider.h"

class GameObject {
public:
	glm::vec3 m_Position = glm::vec3(0.0f,0.0f,0);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 m_Direction = glm::normalize(glm::vec3() - glm::vec3(0.0f, 0, 1.0f));
	glm::vec3 m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Direction));
	glm::vec3 m_Up = glm::cross(m_Direction, m_Right);

	inline virtual void Rotate(glm::vec3 rotation) {
		m_Direction.x = sin(glm::radians(-rotation.x)) * cos(glm::radians(rotation.y));
		m_Direction.y = -sin(glm::radians(rotation.y));
		m_Direction.z = cos(glm::radians(-rotation.x)) * cos(glm::radians(rotation.y));

		m_Direction = glm::normalize(m_Direction);
		m_Right = glm::normalize(glm::cross(m_Direction, m_Up));
		m_rotation = rotation;
		UpdateMat4();
	}
	inline virtual void MoveBy(glm::vec3 position) {
		m_Position += position;
		UpdateCollider(m_Position);
		UpdateMat4();
	}
	inline virtual void UpdateMat4() {}
	inline virtual void UpdateCollider(glm::vec3 position) {}
};