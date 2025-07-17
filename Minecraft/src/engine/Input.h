#pragma once

#include <GLFW/glfw3.h>
#include <tuple>

class Input
{
public:
	static bool GetKeyPressed(GLFWwindow* window, int keycode);
	static bool GetKeyDown(GLFWwindow* window, int keycode);
	static bool GetKeyUp(GLFWwindow* window, int keycode);
	static std::pair<double, double> GetMousePosition(GLFWwindow* window);
	static bool GetMouseDown(GLFWwindow* window, int keycode);
};

