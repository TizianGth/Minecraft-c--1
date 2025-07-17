#pragma once
#include <chrono>

class Time {
public:
	static void Start();

	static float deltaTime;

private:
	static std::chrono::steady_clock::time_point m_CurrentTime;
	static std::chrono::steady_clock::time_point m_PreviousTime;
};