#include "BoxCollider.h"
#include <limits>
#include <math.h>
#include <iostream>

const float BoxCollider::m_Offset = 0.0000001f;

float BoxCollider::SweptAABB(BoxCollider& a, BoxCollider& b, glm::vec3& collisionNormal)
{
	float xInvEntry, yInvEntry, zInvEntry;
	float xInvExit, yInvExit, zInvExit;

	// find the distance between the objects on the near and far sides for both x and y 
	if (a.m_Velocity.x > 0.0f)
	{
		xInvEntry = b.m_Position.x - (a.m_Position.x + a.m_Size.x);
		xInvExit = (b.m_Position.x + b.m_Size.x) - a.m_Position.x;
	}
	else
	{
		xInvEntry = (b.m_Position.x + b.m_Size.x) - a.m_Position.x;
		xInvExit = b.m_Position.x - (a.m_Position.x + a.m_Size.x);
	}

	if (a.m_Velocity.y > 0.0f)
	{
		yInvEntry = b.m_Position.y - (a.m_Position.y + a.m_Size.y);
		yInvExit = (b.m_Position.y + b.m_Size.y) - a.m_Position.y;
	}
	else
	{
		yInvEntry = (b.m_Position.y + b.m_Size.y) - a.m_Position.y;
		yInvExit = b.m_Position.y - (a.m_Position.y + a.m_Size.y);
	}
	if (a.m_Velocity.z > 0.0f)
	{
		zInvEntry = b.m_Position.z - (a.m_Position.z + a.m_Size.z);
		zInvExit = (b.m_Position.z + b.m_Size.z) - a.m_Position.z;
	}
	else
	{
		zInvEntry = (b.m_Position.z + b.m_Size.z) - a.m_Position.z;
		zInvExit = b.m_Position.z - (a.m_Position.z + a.m_Size.z);
	}

	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
	float xEntry, yEntry, zEntry;
	float xExit, yExit, zExit;

	if (a.m_Velocity.x == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / a.m_Velocity.x;
		xExit = xInvExit / a.m_Velocity.x;
	}

	if (a.m_Velocity.y == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / a.m_Velocity.y;
		yExit = yInvExit / a.m_Velocity.y;
	}
	if (a.m_Velocity.z == 0.0f)
	{
		zEntry = -std::numeric_limits<float>::infinity();
		zExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		zEntry = zInvEntry / a.m_Velocity.z;
		zExit = zInvExit / a.m_Velocity.z;
	}

	// find the earliest/latest times of collisionfloat 
	float entryTime = std::max(std::max(xEntry, yEntry), zEntry);
	float exitTime = std::min(std::min(xExit, yExit), zExit);

	// No collision if the first collision occurs after the last collision
	if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f || zEntry > 1.0f) {
		return 1.0f;
	}

	// Calculate collision normal
	if (xEntry > yEntry && xEntry > zEntry) {
		collisionNormal = glm::vec3((a.m_Velocity.x > 0.0f) ? -1.0f : 1.0f, 0.0f, 0.0f);
	}
	else if (yEntry > zEntry) {
		collisionNormal = glm::vec3(0.0f, (a.m_Velocity.y > 0.0f) ? -1.0f : 1.0f, 0.0f);
	}
	else {
		collisionNormal = glm::vec3(0.0f, 0.0f, (a.m_Velocity.z > 0.0f) ? -1.0f : 1.0f);
	}

	return entryTime;
}

int BoxCollider::Sign(int a)
{
	return a >= 0 ? 1 : -1;
}


