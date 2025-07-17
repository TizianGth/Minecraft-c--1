#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include "Line.h"
#include <glm/gtx/string_cast.hpp>

class AABB {
public:
    glm::vec3 m_Min = glm::vec3(0.0f),
        m_Max = glm::vec3(0.0f);


    inline AABB() {}
    inline AABB(glm::vec3 min, glm::vec3 max) {
		m_Min = min;
		m_Max = max;

		//std::cout << "AABB created with min: " << glm::to_string(m_Min) << " and max: " << glm::to_string(m_Max) << std::endl;


    }



};
