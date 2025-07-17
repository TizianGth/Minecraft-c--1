#include "Input.h"

bool Input::GetKeyPressed(GLFWwindow* window, int keycode)
{
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::GetKeyDown(GLFWwindow* window, int keycode)
{
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS;
}

bool Input::GetKeyUp(GLFWwindow* window, int keycode)
{
	return !GetKeyPressed(window, keycode);
}


std::pair<double, double> Input::GetMousePosition(GLFWwindow* window)
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return std::pair<double, double> {xPos, yPos};
}

bool Input::GetMouseDown(GLFWwindow* window, int keycode)
{
	auto state = glfwGetMouseButton(window, keycode);
	return state == GLFW_PRESS;
}
