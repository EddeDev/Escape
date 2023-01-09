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
		m_Data.Width = createInfo.Width;
		m_Data.Height = createInfo.Height;
		m_Data.Title = createInfo.Title;

		if (s_GLFWWindowCount == 0)
		{
			int32 success = glfwInit();
			// Assert
		}

		m_WindowHandle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		if (!m_WindowHandle)
		{
			// Assert
			return;
		}

		s_GLFWWindowCount++;

		glfwGetWindowSize(m_WindowHandle, &m_Data.Width, &m_Data.Height);

		CenterWindow();
		SetCallbacks();

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

	void Window::CenterWindow()
	{
		auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int32 xpos = (videomode->width - m_Data.Width) / 2;
		int32 ypos = (videomode->height - m_Data.Height) / 2;
		glfwSetWindowPos(m_WindowHandle, xpos, ypos);
	}

	void Window::SetCallbacks()
	{
		glfwSetWindowUserPointer(m_WindowHandle, &m_Data);

		glfwSetWindowCloseCallback(m_WindowHandle, [](auto window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.CloseCallbacks)
				callback();
		});

		glfwSetWindowSizeCallback(m_WindowHandle, [](auto window, auto width, auto height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			for (auto& callback : data.ResizeCallbacks)
				callback(width, height);
		});
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