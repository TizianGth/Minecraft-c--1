#pragma once

#include "vector.h"
#include "glm/common.hpp"
#include "Renderer.h"

class Box
{
public:
	Box(glm::vec3 position, glm::vec3 dimensions, glm::vec3 velocity) {
		x = position.x;
		y = position.y;
		z = position.z;

		w = dimensions.x;
		h = dimensions.y;
		d = dimensions.z;

		vx = velocity.x;
		vy = velocity.y;
		vz = velocity.z;

	};

	// position of top-left corner 
	float x, y, z;

	// dimensions 
	float w, h, d;

	// velocity 
	float vx, vy, vz;
};


class BoxCollider {
public:
	BoxCollider() {};

	BoxCollider(glm::vec3 position, glm::vec3 size) : m_Position(position) {

		min.x = position.x - (size.x * 0.5f);
		max.x = position.x + (size.x * 0.5f);
		min.y = position.y - (size.y * 0.5f);
		max.y = position.y + (size.y * 0.5f);
		min.z = position.z - (size.z * 0.5f);
		max.z = position.z + (size.z * 0.5f);

	};


	~BoxCollider() = default;

	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec3 max = glm::vec3(0.0f);
	glm::vec3 min = glm::vec3(0.0f);

	void Update(glm::vec3 position, glm::vec3 size) {
		m_Position = position;
		min.x = position.x - (size.x * 0.5f);
		max.x = position.x + (size.x * 0.5f);
		min.y = position.y - (size.y * 0.5f);
		max.y = position.y + (size.y * 0.5f);
		min.z = position.z - (size.z * 0.5f);
		max.z = position.z + (size.z * 0.5f);
	}

	void SetDisplay() {
		Renderer& r = Renderer::Get();

		r.AddLine(Vector3::Float(Vector3::Float(min.x, min.y, min.z)), Vector3::Float(max.x, min.y, min.z));
		r.AddLine(Vector3::Float(Vector3::Float(min.x, max.y, min.z)), Vector3::Float(max.x, max.y, min.z));

		r.AddLine(Vector3::Float(Vector3::Float(min.x, min.y, max.z)), Vector3::Float(max.x, min.y, max.z));
		r.AddLine(Vector3::Float(Vector3::Float(min.x, max.y, max.z)), Vector3::Float(max.x, max.y, max.z));


		r.AddLine(Vector3::Float(Vector3::Float(max.x, min.y, min.z)), Vector3::Float(max.x, min.y, max.z));
		r.AddLine(Vector3::Float(Vector3::Float(max.x, max.y, min.z)), Vector3::Float(max.x, max.y, max.z));

		r.AddLine(Vector3::Float(Vector3::Float(min.x, min.y, min.z)), Vector3::Float(min.x, min.y, max.z));
		r.AddLine(Vector3::Float(Vector3::Float(min.x, max.y, min.z)), Vector3::Float(min.x, max.y, max.z));


		r.AddLine(Vector3::Float(Vector3::Float(min.x, min.y, min.z)), Vector3::Float(min.x, max.y, min.z));
		r.AddLine(Vector3::Float(Vector3::Float(max.x, min.y, min.z)), Vector3::Float(max.x, max.y, min.z));

		r.AddLine(Vector3::Float(Vector3::Float(max.x, min.y, max.z)), Vector3::Float(max.x, max.y, max.z));
		r.AddLine(Vector3::Float(Vector3::Float(min.x, min.y, max.z)), Vector3::Float(min.x, max.y, max.z));
	}

	bool Colliding(BoxCollider b) {
		return (
			min.x <= b.max.x &&
			max.x >= b.min.x &&
			min.y <= b.max.y &&
			max.y >= b.min.y &&
			min.z <= b.max.z &&
			max.z >= b.min.z
			);
	}

	std::vector<BoxCollider> Colliding(std::vector<BoxCollider> bs) {
		std::vector<BoxCollider> result;
		for (int i = 0; i < bs.size(); i++) {
			if (Colliding(bs[i]))
			{
				result.push_back(bs[i]);
			}
		}
		return result;

	}

	// Swept AABB
	// TODO: Followed tutorial(buggy!!) maybe come back later if needed
	/*
	float SweptAABB(BoxCollider b1, BoxCollider b2, float& normalX, float& normalY, float& normalZ) {
		float xInvEntry, yInvEntry, zInvEntry;
		float xInvExit, yInvExit, zInvExit;

		// find the distance between the objects on the near and far sides for both x and y
		if (b1.velocity.x > 0.0f)
		{
			xInvEntry = b2.position.x - (b1.position.x + b1.dimension.x);
			xInvExit = (b2.position.x + b2.dimension.x) - b1.position.x;
		}
		else
		{
			xInvEntry = (b2.position.x + b2.dimension.x) - b1.position.x;
			xInvExit = b2.position.x - (b1.position.x + b1.dimension.x);
		}

		if (b1.velocity.y > 0.0f)
		{
			yInvEntry = b2.position.y - (b1.position.y + b1.dimension.y);
			yInvExit = (b2.position.y + b2.dimension.y) - b1.position.y;
		}
		else
		{
			yInvEntry = (b2.position.y + b2.dimension.y) - b1.position.y;
			yInvExit = b2.position.y - (b1.position.y + b1.dimension.y);
		}

		if (b1.velocity.z > 0.0f)
		{
			zInvEntry = b2.position.z - (b1.position.z + b1.dimension.z);
			zInvExit = (b2.position.z + b2.dimension.z) - b1.position.z;
		}
		else
		{
			zInvEntry = (b2.position.z + b2.dimension.z) - b1.position.z;
			zInvExit = b2.position.z - (b1.position.z + b1.dimension.z);
		}

		// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
		float xEntry, yEntry, zEntry;;
		float xExit, yExit, zExit;

		if (b1.velocity.x == 0.0f)
		{
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			xEntry = xInvEntry / b1.velocity.x;
			xExit = xInvExit / b1.velocity.x;
		}

		if (b1.velocity.y == 0.0f)
		{
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			yEntry = yInvEntry / b1.velocity.y;
			yExit = yInvExit / b1.velocity.y;
		}

		if (b1.velocity.z == 0.0f)
		{
			zEntry = -std::numeric_limits<float>::infinity();
			zExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			zEntry = zInvEntry / b1.velocity.z;
			zExit = zInvExit / b1.velocity.z;
		}

		// find the earliest/latest times of collisionfloat
		float entryTime = std::max(std::max(xEntry, yEntry), zEntry);
		float exitTime = std::min(std::min(xExit, yExit), zExit);

		/*std::cout << xEntry << std::endl;
		std::cout << yEntry << std::endl;
		std::cout << zEntry << std::endl;

		std::cout << xExit << std::endl;
		std::cout << yExit << std::endl;
		std::cout << zExit << std::endl << std::endl;

		// if there was no collision
		if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f || zEntry > 1.0f)
		{
			normalX = 0.0f;
			normalY = 0.0f;
			normalZ = 0.0f;
			return 1.0f;
		}
		else // if there was a collision
		{
			if (xEntry > yEntry) {
				if (xEntry > zEntry) {
					normalX = -Sign(b1.velocity.x);
					normalY = 0.0f;
					normalZ = 0.0f;
				}
				else {
					normalZ = -Sign(b1.velocity.z);
					normalX = 0.0f;
					normalY = 0.0f;
				}
			}
			else {
				if (yEntry > zEntry) {
					normalY = -Sign(b1.velocity.y);
					normalX = 0.0f;
					normalZ = 0.0f;

				}
				else {
					normalZ = -Sign(b1.velocity.z);
					normalX = 0.0f;
					normalY = 0.0f;
				}
			}
			return entryTime;
		}
	} */


	glm::vec3 RoundDec(glm::vec3 v, int decimals) {
		int h = std::pow(10, decimals);
		float x = (float)((int)(v.x * h + 0.5f)) / h;
		float y = (float)((int)(v.y * h + 0.5f)) / h;
		float z = (float)((int)(v.z * h + 0.5f)) / h;

		return glm::vec3(x, y, z);
	}

	glm::vec3 GetCollidingNormals(BoxCollider b, glm::vec3 velocity) {

		float pushbackForce = -0.000f;
		glm::vec3 middle = b.m_Position - m_Position;


		if (std::abs(middle.x) > std::abs(middle.y) && std::abs(middle.x) > std::abs(middle.z)) {
			if ((m_Position.x + velocity.x > m_Position.x && middle.x < 0) || (m_Position.x + velocity.x < m_Position.x && middle.x > 0)) {
				return glm::vec3(1, 1, 1);
			}
			else {
				return glm::vec3(pushbackForce, 1, 1);
			}
		}
		if (std::abs(middle.y) > std::abs(middle.x) && std::abs(middle.y) > std::abs(middle.z)) {
			if ((m_Position.y + velocity.y > m_Position.y && middle.y < 0) || (m_Position.y + velocity.y < m_Position.y && middle.y > 0)) {
				return glm::vec3(1, 1, 1);
			}
			else {
				return glm::vec3(1, pushbackForce, 1);
			}

		}
		else {
			if ((m_Position.z + velocity.z > m_Position.z && middle.z < 0) || (m_Position.z + velocity.z < m_Position.z && middle.z > 0)) {
				return glm::vec3(1, 1, 1);
			}
			else {
				return glm::vec3(1, 1, pushbackForce);
			}
		}
	}
	/// <summary>
	/// Finding nearest collider to avoid slipping through colliders
	/// TODO: Account for different collider sizes!!
	/// </summary>
	glm::vec3 GetCollidingNormals(std::vector<BoxCollider> bs, glm::vec3 velocity) {
		BoxCollider& nearest = bs[0];
		for (int i = 0; i < bs.size(); i++) {
			glm::vec3 newMiddle = bs[i].m_Position - m_Position;
			glm::vec3 oldMiddle = nearest.m_Position - m_Position;

			float newMiddleSum = std::abs(newMiddle.x) + std::abs(newMiddle.y) + std::abs(newMiddle.z);
			float oldMiddleSum = std::abs(oldMiddle.x) + std::abs(oldMiddle.y) + std::abs(oldMiddle.z);
			if (newMiddleSum < oldMiddleSum) {
				nearest = bs[i];
			}
		}

		//std::cout << nearest.m_Position.x << std::endl;
		return GetCollidingNormals(nearest, velocity);
	}

	int Sign(int a) { return a >= 0 ? 1 : -1; }

};
