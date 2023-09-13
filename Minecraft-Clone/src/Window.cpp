#include <GL/glew.h>
#include "Window.h"
#include "Input.h"

#include <iostream>

Window::Window(std::string title, unsigned int width, unsigned int height, bool fullScreen)
	: m_window(nullptr), m_wndTitle(title), m_wndWidth(width), m_wndHeight(height), m_isFullScreen(fullScreen)
{
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW!" << std::endl;
		__debugbreak();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_wndWidth, m_wndHeight, m_wndTitle.c_str(), nullptr, nullptr);

	if (!m_window) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		__debugbreak();
	}

	SetFullScreen(m_isFullScreen);

	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW!" << std::endl;
		glfwTerminate();
		__debugbreak();
	}

	glViewport(0, 0, m_wndWidth, m_wndHeight);	

	Input::InitializeInputClass(this);
	glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(m_window, Input::MouseMoveCallback);
	glfwSetScrollCallback(m_window, Input::MouseScrollCallback);
	glfwSetKeyCallback(m_window, Input::KeyboardCallback);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Window::IsFullScreen()
{
	return m_isFullScreen;
}

void Window::SetFullScreen(bool flag)
{
	if (m_isFullScreen != flag) {
		m_isFullScreen = flag;
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (m_isFullScreen)
			glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else
			glfwSetWindowMonitor(m_window, NULL, mode->width / 2 - m_wndWidth / 2, mode->height / 2 - m_wndHeight / 2, m_wndWidth, m_wndHeight, 0);
	}
}

void Window::ToggleFullScreen()
{
	SetFullScreen(!IsFullScreen());
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
