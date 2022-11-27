#pragma once

#include <vector>

// Maybe struct
class Blocktype {
public:
	vector3Int m_Position;
	int m_BlockType;

	Blocktype(vector3Int position, int blocktype) {
		m_Position = position;
		m_BlockType = blocktype;
	}

	Blocktype() {
		m_Position = vector3Int(0, 0, 0);
		m_BlockType = 0;
	}

	void Set(vector3Int position, int blocktype) {
		m_Position = position;
		m_BlockType = blocktype;
	}
};