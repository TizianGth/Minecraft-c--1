#pragma once
#include <GLFW/glfw3.h>

class Application {
public:
	static GLFWwindow* GetWindow();
	static int GetWindowWidth();
	static int GetWindowHeight();
};
