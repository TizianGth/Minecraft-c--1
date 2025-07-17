#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>

class AABB {
public:
public:
    glm::vec3 m_Position, m_Size;

    AABB(glm::vec3 position, glm::vec3 size) : m_Position(position), m_Size(size) {}

    bool checkCollision(const AABB& other) const {
        // Check for collision along the x-axis
        bool collisionX = (m_Position.x < other.m_Position.x + other.m_Size.x) &&
            (m_Position.x + m_Size.x > other.m_Position.x);

        // Check for collision along the y-axis
        bool collisionY = (m_Position.y < other.m_Position.y + other.m_Size.y) &&
            (m_Position.y + m_Size.y > other.m_Position.y);

        // Check for collision along the z-axis
        bool collisionZ = (m_Position.z < other.m_Position.z + other.m_Size.z) &&
            (m_Position.z + m_Size.z > other.m_Position.z);

        // Return true if there is a collision along all axes
        return collisionX && collisionY && collisionZ;
    }

    void move(const glm::vec3& delta) {
        m_Position += delta;

    }



    void resolveCollision(AABB& other) {
        float xOverlap = std::min(m_Position.x + m_Size.x, other.m_Position.x + other.m_Size.x) - std::max(m_Position.x, other.m_Position.x);
        float yOverlap = std::min(m_Position.y + m_Size.y, other.m_Position.y + other.m_Size.y) - std::max(m_Position.y, other.m_Position.y);
        float zOverlap = std::min(m_Position.z + m_Size.z, other.m_Position.z + other.m_Size.z) - std::max(m_Position.z, other.m_Position.z);


        // Choose the axis with the smallest overlap
        if (xOverlap < yOverlap && xOverlap < zOverlap) {
            // Resolve collision along the x-axis
            if (m_Position.x < other.m_Position.x) {
                m_Position.x -= xOverlap;
            }
            else {
                m_Position.x += xOverlap;
            }
        }
        else if (yOverlap < zOverlap) {
            // Resolve collision along the y-axis
            if (m_Position.y < other.m_Position.y) {
                m_Position.y -= yOverlap;
            }
            else {
                m_Position.y += yOverlap;
            }
        }
        else {
            // Resolve collision along the z-axis
            if (m_Position.z < other.m_Position.z) {
                m_Position.z -= zOverlap;
            }
            else {
                m_Position.z += zOverlap;
            }
        }
    }
};
