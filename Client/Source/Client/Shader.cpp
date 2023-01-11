#include "ClientPCH.h"
#include "Shader.h"

#include "FileUtils.h"

#include <glad/glad.h>

namespace esc {
	
	namespace Utils {

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

		static uint32 ShaderDataTypeSize(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:  return sizeof(float) * 1;
			case ShaderDataType::Float2: return sizeof(float) * 2;
			case ShaderDataType::Float3: return sizeof(float) * 3;
			case ShaderDataType::Float4: return sizeof(float) * 4;
			case ShaderDataType::Int:    return sizeof(int32) * 1;
			case ShaderDataType::Int2:   return sizeof(int32) * 2;
			case ShaderDataType::Int3:   return sizeof(int32) * 3;
			case ShaderDataType::Int4:   return sizeof(int32) * 4;
			case ShaderDataType::Mat3:   return sizeof(float) * 3 * 3;
			case ShaderDataType::Mat4:   return sizeof(float) * 4 * 4;
			}
			std::cerr << "Unknown shader data type!" << std::endl;
			return 0;
		}

		static uint32 ShaderDataTypeComponentCount(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
			case ShaderDataType::Mat3:   return 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4;
			}
			std::cerr << "Unknown shader data type!" << std::endl;
			return 0;
		}

		static ShaderDataType ShaderDataTypeFromOpenGLType(uint32 type)
		{
			switch (type)
			{
			case GL_FLOAT:      return ShaderDataType::Float;
			case GL_FLOAT_VEC2: return ShaderDataType::Float2;
			case GL_FLOAT_VEC3: return ShaderDataType::Float3;
			case GL_FLOAT_VEC4: return ShaderDataType::Float4;
			case GL_INT:        return ShaderDataType::Int;
			case GL_INT_VEC2:   return ShaderDataType::Int2;
			case GL_INT_VEC3:   return ShaderDataType::Int3;
			case GL_INT_VEC4:   return ShaderDataType::Int4;
			case GL_FLOAT_MAT3: return ShaderDataType::Mat3;
			case GL_FLOAT_MAT4: return ShaderDataType::Mat4;
			}
			std::cerr << "Unknown shader data type!" << std::endl;
			return ShaderDataType::None;
		}

	}

	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		std::unordered_map<ShaderStage, std::string> sources;
		sources[ShaderStage::Vertex] = FileUtils::ReadFile(vertexShaderPath);
		sources[ShaderStage::Fragment] = FileUtils::ReadFile(fragmentShaderPath);

		Compile(sources);
		Reflect();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform(const ShaderUniform& uniform, const void* data)
	{
		switch (uniform.Type)
		{
		case ShaderDataType::Float:
		{
			glProgramUniform1fv(m_ProgramID, uniform.Location, 1, (const float*)data);
			break;
		}
		case ShaderDataType::Float2:
		{
			glProgramUniform2fv(m_ProgramID, uniform.Location, 1, (const float*)data);
			break;
		}
		case ShaderDataType::Float3:
		{
			glProgramUniform3fv(m_ProgramID, uniform.Location, 1, (const float*)data);
			break;
		}
		case ShaderDataType::Float4:
		{
			glProgramUniform4fv(m_ProgramID, uniform.Location, 1, (const float*)data);
			break;
		}
		case ShaderDataType::Int:
		{
			glProgramUniform1iv(m_ProgramID, uniform.Location, 1, (const int32*)data);
			break;
		}
		case ShaderDataType::Int2:
		{
			glProgramUniform2iv(m_ProgramID, uniform.Location, 1, (const int32*)data);
			break;
		}
		case ShaderDataType::Int3:
		{
			glProgramUniform3iv(m_ProgramID, uniform.Location, 1, (const int32*)data);
			break;
		}
		case ShaderDataType::Int4:
		{
			glProgramUniform4iv(m_ProgramID, uniform.Location, 1, (const int32*)data);
			break;
		}
		case ShaderDataType::Mat3:
		{
			glProgramUniformMatrix3fv(m_ProgramID, uniform.Location, 1, GL_FALSE, (const float*)data);
			break;
		}
		case ShaderDataType::Mat4:
		{
			glProgramUniformMatrix4fv(m_ProgramID, uniform.Location, 1, GL_FALSE, (const float*)data);
			break;
		}
		}
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

	void Shader::Reflect()
	{
		// Attributes
		{
			int32 count = 0;
			glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTES, &count);

			int32 maxLength = 0;
			glGetProgramInterfaceiv(m_ProgramID, GL_PROGRAM_INPUT, GL_MAX_NAME_LENGTH, &maxLength);

			std::vector<char> name(maxLength);

			for (int32 i = 0; i < count; i++)
			{
				int32 length;
				int32 size;
				uint32 type;
				glGetActiveAttrib(m_ProgramID, (uint32)i, maxLength, &length, &size, &type, name.data());

				ShaderAttribute& attribute = m_InputLayout.Attributes.emplace_back();
				attribute.Name = name.data();
				attribute.Index = glGetAttribLocation(m_ProgramID, name.data());
				attribute.Type = Utils::ShaderDataTypeFromOpenGLType(type);
				attribute.Size = Utils::ShaderDataTypeSize(attribute.Type);
				attribute.ComponentCount = Utils::ShaderDataTypeComponentCount(attribute.Type);
			}

			std::sort(m_InputLayout.Attributes.begin(), m_InputLayout.Attributes.end(), [](const ShaderAttribute& a, const ShaderAttribute& b)
			{
				return a.Index < b.Index;
			});

			uint32 offset = 0;
			for (int32 i = 0; i < count; i++)
			{
				ShaderAttribute& attribute = m_InputLayout.Attributes[i];
				attribute.Offset = offset;
				offset += attribute.Size;
			}

			m_InputLayout.Stride = offset;
		}
		
		// Uniforms
		{
			int32 count = 0;
			glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORMS, &count);

			int32 maxLength = 0;
			glGetProgramInterfaceiv(m_ProgramID, GL_UNIFORM, GL_MAX_NAME_LENGTH, &maxLength);

			std::vector<char> name(maxLength);

			for (int32 i = 0; i < count; i++)
			{
				int32 length;
				int32 size;
				uint32 type;
				glGetActiveUniform(m_ProgramID, (uint32)i, maxLength, &length, &size, &type, name.data());
			
				ShaderUniform& uniform = m_Uniforms[name.data()];
				uniform.Name = name.data();
				uniform.Type = Utils::ShaderDataTypeFromOpenGLType(type);
				uniform.Size = Utils::ShaderDataTypeSize(uniform.Type);
				uniform.ComponentCount = Utils::ShaderDataTypeComponentCount(uniform.Type);
				uniform.Location = glGetUniformLocation(m_ProgramID, uniform.Name.c_str());
			}
		}
	}


}