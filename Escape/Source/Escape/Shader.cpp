#include "EscapePCH.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Escape {
	
	namespace Utils {

		static std::string ReadFile(const std::string& filepath)
		{
			std::string result;
			std::ifstream in(filepath, std::ios::in | std::ios::binary);
			if (in)
			{
				in.seekg(0, std::ios::end);
				std::streampos size = in.tellg();
				in.seekg(0, std::ios::beg);
				result.resize(size);
				in.read(result.data(), size);
			}
			else
			{
				std::cerr << "Could not open file '" << filepath << "'!" << std::endl;
			}
			in.close();
			return result;
		}

		static uint32 OpenGLShaderStage(ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:   return GL_VERTEX_SHADER;
			case ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
			}
			std::cerr << "Unknown shader stage!" << std::endl;
			return 0;
		}

		static const char* ShaderStageToString(ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:   return "Vertex";
			case ShaderStage::Fragment: return "Fragment";
			}
			std::cerr << "Unknown shader stage!" << std::endl;
			return "";
		}

	}

	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		std::unordered_map<ShaderStage, std::string> sources;
		sources[ShaderStage::Vertex] = Utils::ReadFile(vertexShaderPath);
		sources[ShaderStage::Fragment] = Utils::ReadFile(fragmentShaderPath);

		Compile(sources);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::Compile(const std::unordered_map<ShaderStage, std::string>& sources)
	{
		std::unordered_map<ShaderStage, uint32> shaderIDs;

		for (auto& [stage, source] : sources)
		{
			uint32 shaderID = glCreateShader(Utils::OpenGLShaderStage(stage));

			const char* sourceCStr = source.c_str();
			glShaderSource(shaderID, 1, &sourceCStr, 0);

			glCompileShader(shaderID);
			int32 isCompiled = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int32 maxLength = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog.data());

				std::cout << "Failed to compile " << Utils::ShaderStageToString(stage) << " shader!\n" << infoLog.data() << std::endl;
				glDeleteShader(shaderID);
				continue;
			}

			shaderIDs[stage] = shaderID;
		}

		m_ProgramID = glCreateProgram();
		for (auto& [stage, shaderID] : shaderIDs)
			glAttachShader(m_ProgramID, shaderID);

		glLinkProgram(m_ProgramID);
		int32 isLinked = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int32 maxLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, infoLog.data());

			std::cout << "Failed to link shader program!\n" << infoLog.data() << std::endl;

			for (auto& [stage, shaderID] : shaderIDs)
				glDeleteShader(shaderID);

			glDeleteProgram(m_ProgramID);
		}

		for (auto& [stage, shaderID] : shaderIDs)
			glDetachShader(m_ProgramID, shaderID);
	}

}