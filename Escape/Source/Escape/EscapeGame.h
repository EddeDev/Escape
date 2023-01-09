#pragma once

#include "Window.h"

#include "Shader.h"

namespace Escape {

	class EscapeGame
	{
	public:
		EscapeGame();
		~EscapeGame();

		void Run();
	private:
		Ref<Window> m_Window;
		Ref<Shader> m_Shader;
		bool m_Running = true;
	};

}