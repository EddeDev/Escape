#pragma once

#include "Shader.h"
#include "Texture.h"

namespace esc {

	class ResourceManager
	{
	public:
		static void Init();
		static void Shutdown();

		static Ref<Shader> GetShader(const std::string& name);
		static Ref<Texture> GetTexture(const std::string& name);
	private:
		inline static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
		inline static std::unordered_map<std::string, Ref<Texture>> s_Textures;
	};

}