#pragma once

#include <vector>

// Maybe struct
class Blocktype {
public:
	glm::vec3 m_Position;
	int m_BlockType;

	Blocktype(glm::vec3 position, int blocktype) {
		m_Position = position;
		m_BlockType = blocktype;
	}

	Blocktype() {
		m_Position = glm::vec3(0, 0, 0);
		m_BlockType = 0;
	}

	void Set(glm::vec3&position, int blocktype) {
		m_Position = position;
		m_BlockType = blocktype;
	}
};