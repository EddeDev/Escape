#pragma once

struct GLFWwindow;

namespace Escape {

	struct WindowCreateInfo
	{
		int32 Width, Height;
		std::string Title;
	};

	class Window : public ReferenceCounted
	{
	public:
		Window(const WindowCreateInfo& createInfo);
		virtual ~Window();
		
		void PollEvents();
		void SwapBuffers();

		void AddCloseCallback(const std::function<void()>& callback) { m_Data.CloseCallbacks.push_back(callback); }
		void AddResizeCallback(const std::function<void(int32, int32)>& callback) { m_Data.ResizeCallbacks.push_back(callback); }
	private:
		void Init(const WindowCreateInfo& createInfo);
		void Shutdown();

		void CenterWindow();
		void SetCallbacks();
	private:
		GLFWwindow* m_WindowHandle = nullptr;

		struct WindowData
		{
			int32 Width, Height;
			std::string Title;

			std::vector<std::function<void()>> CloseCallbacks;
			std::vector<std::function<void(int32, int32)>> ResizeCallbacks;
		} m_Data;
	};

}