#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow*		m_window;
	std::string		m_wndTitle;
	unsigned int	m_wndWidth;
	unsigned int	m_wndHeight;
	bool			m_isFullScreen;

public:
	Window(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	~Window();

	bool IsFullScreen();
	GLFWwindow** GetGLFWWindow() { return &m_window; }

	void SetFullScreen(bool flag);
	void ToggleFullScreen();

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};