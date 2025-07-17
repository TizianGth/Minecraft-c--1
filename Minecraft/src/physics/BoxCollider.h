#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>


class BoxCollider
{
public:
	float SweptAABB(BoxCollider& a, BoxCollider& b, glm::vec3& collisionNormal);


	glm::vec3 m_Velocity = glm::vec3(0.0f);
	glm::vec3 m_Position = glm::vec3(0.0f); // bottom
	glm::vec3 m_Size = glm::vec3(0.0f);

	BoxCollider() {};
	BoxCollider(const glm::vec3& position, const glm::vec3& size) : m_Position(position), m_Size(size) {};
	BoxCollider(const glm::vec3& position, const glm::vec3& size, const glm::vec3& velocity) : m_Position(position), m_Size(size), m_Velocity(velocity) {};

private:
	static int Sign(int a);

	static const float m_Offset;
};

