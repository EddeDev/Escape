#include "ClientPCH.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace esc {

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

		if (createInfo.Fullscreen)
		{
			auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			m_Data.Width = videomode->width;
			m_Data.Height = videomode->height;
			m_WindowHandle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
		}
		else
		{
			m_WindowHandle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

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

		glfwSetKeyCallback(m_WindowHandle, [](auto window, auto key, auto scancode, auto action, auto mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			// TODO: Temp bugfix	
			if (key < 0)
				return;

			switch (action)
			{
			case GLFW_PRESS:
			{
				for (auto& callback : data.KeyPressCallbacks)
					callback(key);
				break;
			}
			case GLFW_RELEASE:
			{
				for (auto& callback : data.KeyReleaseCallbacks)
					callback(key);
				break;
			}
			case GLFW_REPEAT:
			{
				for (auto& callback : data.KeyRepeatCallbacks)
					callback(key);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_WindowHandle, [](auto window, auto button, auto action, auto mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				for (auto& callback : data.MouseButtonPressCallbacks)
					callback(button);
				break;
			}
			case GLFW_RELEASE:
			{
				for (auto& callback : data.MouseButtonReleaseCallbacks)
					callback(button);
				break;
			}
			}
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

	float Window::GetTime() const
	{
		return static_cast<float>(glfwGetTime());
	}

	bool Window::IsKeyDown(int32 key) const
	{
		int32 state = glfwGetKey(m_WindowHandle, key);
		return state == GLFW_PRESS;
	}

	bool Window::IsMouseButtonDown(int32 button) const
	{
		int32 state = glfwGetMouseButton(m_WindowHandle, button);
		return state == GLFW_PRESS;
	}

}