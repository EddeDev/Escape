#pragma once

#include "Window.h"

namespace Escape {

	class EscapeGame
	{
	public:
		EscapeGame();
		~EscapeGame();

		void Run();
	private:
		Ref<Window> m_Window;
		bool m_Running = true;
	};

}