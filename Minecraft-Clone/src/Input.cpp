#include "Input.h"

#include <iostream>

Window* Input::m_window = nullptr;
double Input::m_scrollOffset = 0.0;
double Input::m_mouseX = 0.0;
double Input::m_mouseY = 0.0;

double Input::m_mouseXOffset = 0.0;
double Input::m_mouseYOffset = 0.0;
double Input::m_mouseXLast = 0.0;
double Input::m_mouseYLast = 0.0;

bool Input::m_firstMouse = true;

void Input::InitializeInputClass(Window* window)
{
	m_window = window;
}

void Input::MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	m_mouseX = xPos;
	m_mouseY = yPos;

	if (m_firstMouse)
	{
		m_mouseXLast = m_mouseX;
		m_mouseYLast = m_mouseY;
		m_firstMouse = false;
	}

	m_mouseXOffset += m_mouseX - m_mouseXLast;
	m_mouseYOffset += m_mouseYLast - m_mouseY;

	m_mouseXLast = m_mouseX;
	m_mouseYLast = m_mouseY;
}

void Input::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Input::m_scrollOffset = yOffset;
}

void Input::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;

		case GLFW_KEY_F1:
			m_window->ToggleFullScreen();
			break;
		}
	}
}

bool Input::IsKeyPressed(int key)
{
	return glfwGetKey(*(m_window->GetGLFWWindow()), key) == GLFW_PRESS;
}

double Input::GetMouseXOffset()
{
	double temp = m_mouseXOffset;
	m_mouseXOffset = 0.0;
	return temp;
}

double Input::GetMouseYOffset()
{
	double temp = m_mouseYOffset;
	m_mouseYOffset = 0.0;
	return temp;
}