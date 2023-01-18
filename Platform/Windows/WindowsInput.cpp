#include "WindowsInput.h"
#include "Application.h"
#include <iostream>
#include <GLFW/glfw3.h>

Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto window = Application::GetWindow();
	auto state = glfwGetKey(window ,keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button)
{
	auto window = Application::GetWindow();
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

float WindowsInput::GetMouseXImpl()
{
	return GetMousePositionImpl().x;
}

float WindowsInput::GetMouseYImpl()
{
	return GetMousePositionImpl().y;
}

Vector2::Double WindowsInput::GetMousePositionImpl()
{
	auto window = Application::GetWindow();
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	
	return Vector2::Double(xPos, yPos);
}

