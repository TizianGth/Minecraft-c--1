#include "Input.h"


bool Input::m_KeyDown[65536];

bool Input::GetKeyPressed(GLFWwindow* window, int keycode)
{
	auto state = glfwGetKey(window, keycode);
	Input::m_KeyDown[keycode] = state == GLFW_PRESS || state == GLFW_REPEAT;
	return Input::m_KeyDown[keycode];
}

bool Input::GetKeyDown(GLFWwindow* window, int keycode)
{
	auto state = glfwGetKey(window, keycode);

	if (state != GLFW_PRESS) {
		Input::m_KeyDown[keycode] = false;
		return false;
	}

	if (Input::m_KeyDown[keycode]) {
		return false;
	}

	Input::m_KeyDown[keycode] = true;
	return true;
}

bool Input::GetKeyUp(GLFWwindow* window, int keycode)
{
	return false;
}


std::pair<double, double> Input::GetMousePosition(GLFWwindow* window)
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return std::pair<double, double> {xPos, yPos};
}