#pragma once

struct GLFWwindow;

namespace Escape {

	struct WindowCreateInfo
	{
		uint32 Width, Height;
		std::string Title;
	};

	class Window : public ReferenceCounted
	{
	public:
		Window(const WindowCreateInfo& createInfo);
		virtual ~Window();
		
		void PollEvents();
		void SwapBuffers();
	private:
		void Init(const WindowCreateInfo& createInfo);
		void Shutdown();
	private:
		GLFWwindow* m_WindowHandle = nullptr;
	};

}