#pragma once

struct Faces {
	enum FacesLayout {
		right = 0,
		left = 1,
		top = 2,
		bottom = 3,
		front = 4,
		back = 5
	};


	bool faces[6] = {false, false, false, false, false, false};

	inline int Count() {
		int result = 0;
		for (int i = 0; i < 6; i++) {
			if (faces[i]) {
				result++;
			}
		}
		return result;
	}
};