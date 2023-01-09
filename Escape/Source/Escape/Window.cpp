#include "EscapePCH.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Escape {

	static uint32 s_GLFWWindowCount = 0;

	Window::Window(const WindowCreateInfo& createInfo)
	{
		Init(createInfo);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowCreateInfo& createInfo)
	{
		if (s_GLFWWindowCount == 0)
		{
			int32 success = glfwInit();
			// Assert
		}

		m_WindowHandle = glfwCreateWindow(createInfo.Width, createInfo.Height, createInfo.Title.c_str(), nullptr, nullptr);
		if (!m_WindowHandle)
		{
			// Assert
			return;
		}

		s_GLFWWindowCount++;

		glfwMakeContextCurrent(m_WindowHandle);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwShowWindow(m_WindowHandle);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_WindowHandle);
		s_GLFWWindowCount--;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}