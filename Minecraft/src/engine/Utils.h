#pragma once

class Utils {
public:
	static int Sign(float f) {
		return (0.0f < f) - (f < 0.0f);
	};
	/*
	static float HighLow(float f) {
		if (f < 0) {
			return std::ceil()
		}
	}*/
};