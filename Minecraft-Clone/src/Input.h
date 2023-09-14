#pragma once

#include "Window.h"
#include "Camera.h"

class Input
{
	static Window* m_window;
	static double m_mouseX, m_mouseY;
	static double m_mouseXLast, m_mouseYLast;
	static double m_scrollOffset;

	static Camera* m_camera;

	static double m_mouseXOffset, m_mouseYOffset;

	static bool m_firstMouse;
private:
	Input();

public:
	static void InitializeInputClass(Window* window);

	static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
	static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool IsKeyPressed(int key);

	static double GetScrollOffset() { return m_scrollOffset; };
	static double GetMousePosX() { return m_mouseX; };
	static double GetMousePosY() { return m_mouseY; };

	static void SetCameraBinding(Camera* camera) { m_camera = camera; };
};