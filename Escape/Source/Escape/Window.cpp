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
			if (!glfwInit())
			{
				std::cout << "Unable to initialize GLFW!" << std::endl;
				return;
			}
		}

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef ESCAPE_BUILD_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		m_WindowHandle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		if (!m_WindowHandle)
		{
			std::cout << "Failed to create GLFW window!" << std::endl;
			return;
		}

		s_GLFWWindowCount++;

		glfwGetWindowSize(m_WindowHandle, &m_Data.Width, &m_Data.Height);

		CenterWindow();
		SetCallbacks();

		glfwMakeContextCurrent(m_WindowHandle);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

#ifdef ESCAPE_BUILD_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback([](uint32 source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam)
		{
			if (id == 131185)
				return;
			std::cerr << "[OpenGL] " << message << " (" << id << ")" << std::endl;
			__debugbreak();
		}, this);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

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