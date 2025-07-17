#pragma once

#include <tuple>

namespace Face {
	static const std::tuple<int, int, int> m_TextCoords[8] = {
		std::tuple<int, int, int> {-1,-1,1},
		std::tuple<int, int, int> {1,-1,1},
		std::tuple<int, int, int> {1,1,1},
		std::tuple<int, int, int> {-1,1,1},
		std::tuple<int, int, int> {-1,-1,-1},
		std::tuple<int, int, int> {1,-1,-1},
		std::tuple<int, int, int> {1,1,-1},
		std::tuple<int, int, int> {-1,1,-1}
	};

	static const std::tuple<int, int, int> m_Directions[6] = {
	std::tuple<int, int, int> {1,0,0},
	std::tuple<int, int, int> {-1,0,0},
	std::tuple<int, int, int> {0,1,0},
	std::tuple<int, int, int> {0,-1,0},
	std::tuple<int, int, int> {0,0,1},
	std::tuple<int, int, int> {0,0,-1}
	};

	enum m_Layout {
		right = 0,
		left = 1,
		top = 2,
		bottom = 3,
		front = 4,
		back = 5
	};

}

class Faces {
public:
	bool m_Faces[6] = {
		false,
		false,
		false,
		false,
		false,
		false
	};

	inline int Count() {
		int c = 0;
		for (bool b : m_Faces) {
			if(b)
				c++;
		}
		
		return c;
	};
};