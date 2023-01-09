#pragma once

namespace Escape {

	enum class ShaderStage : uint8
	{
		None = 0,
		Vertex,
		Fragment
	};

	class Shader : public ReferenceCounted
	{
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		virtual ~Shader();
	private:
		void Compile(const std::unordered_map<ShaderStage, std::string>& sources);
	private:
		uint32 m_ProgramID = 0;
	};

}