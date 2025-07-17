#pragma once

#include <unordered_map>

struct Biomes {
	struct Plains {
	public:

		enum BlockStartHeight {
			WATER = 20,
			SAND = WATER + 1,
		};
	};
};

struct Settings {
public:
	enum BlockTypes {
		INVALID = -1, AIR = 0, WATER = 1, DIRT = 2, GRASS = 3, SAND = 4
	};
	static const std::unordered_map<BlockTypes, bool> RenderRegardless;
};


