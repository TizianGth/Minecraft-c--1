#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>

class GameObject {
public:
	glm::vec3 m_Position = glm::vec3(0.0f,0.0f,1.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 m_Direction = glm::normalize(m_Position - glm::vec3(0, 0, 0));
	glm::vec3 m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Direction));
	glm::vec3 m_Up = glm::cross(m_Direction, m_Right);

	inline virtual void Rotate(glm::vec3 rotation) {
		m_Direction.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		m_Direction.y = sin(glm::radians(rotation.y));
		m_Direction.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));

		m_Direction = glm::normalize(m_Direction);
		m_rotation += rotation;
		UpdateMat4();
	}
	inline virtual void UpdateMat4() {}
};