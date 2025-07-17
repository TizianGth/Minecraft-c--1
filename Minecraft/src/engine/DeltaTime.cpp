#include "DeltaTime.h"

std::chrono::steady_clock::time_point Time::m_CurrentTime = std::chrono::high_resolution_clock::now();
std::chrono::steady_clock::time_point Time::m_PreviousTime = std::chrono::high_resolution_clock::now();
float Time::deltaTime = 0;

void Time::Start()
{
	deltaTime = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_PreviousTime).count();

	m_PreviousTime = std::chrono::high_resolution_clock::now();
}

