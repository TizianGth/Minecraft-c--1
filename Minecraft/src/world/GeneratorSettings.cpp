#include "GeneratorSettings.h"

const std::unordered_map<Settings::BlockTypes, bool> Settings::RenderRegardless = {
	{BlockTypes::INVALID, true},
	{ BlockTypes::AIR, true },
	{ BlockTypes::WATER, true },
};