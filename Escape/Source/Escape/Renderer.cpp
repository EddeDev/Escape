#include "EscapePCH.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace Escape {

	void Renderer::SetViewport(int32 x, int32 y, int32 width, int32 height)
	{
		glViewport(0, 0, width, height);
	}

}