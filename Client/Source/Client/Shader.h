#pragma once

namespace esc {

	enum class ShaderStage : uint8
	{
		None = 0,
		Vertex,
		Fragment
	};

	enum class ShaderDataType : uint8
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4
	};

	struct ShaderAttribute
	{
		std::string Name;
		uint32 Index = 0;
		uint32 Size = 0;
		uint32 Offset = 0;
		uint32 ComponentCount = 0;
		ShaderDataType Type = ShaderDataType::None;
	};

	struct ShaderInputLayout
	{
		uint32 Stride = 0;
		std::vector<ShaderAttribute> Attributes;

		std::vector<ShaderAttribute>::iterator begin() { return Attributes.begin(); }
		std::vector<ShaderAttribute>::iterator end() { return Attributes.end(); }
		std::vector<ShaderAttribute>::const_iterator begin() const { return Attributes.cbegin(); }
		std::vector<ShaderAttribute>::const_iterator end() const { return Attributes.cend(); }
	};

	struct ShaderUniform
	{
		std::string Name;
		uint32 Size = 0;
		uint32 ComponentCount = 0;
		uint32 Location = 0;
		ShaderDataType Type = ShaderDataType::None;
	};

	struct ShaderInfo
	{
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
	};

	class Shader : public ReferenceCounted
	{
	public:
		Shader(const ShaderInfo& info);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform(const ShaderUniform& uniform, const void* data);

		template<typename T>
		void SetUniform(const std::string& name, const T& value)
		{
			if (m_Uniforms.find(name) == m_Uniforms.end())
			{
				std::cerr << "Could not find uniform " << name << " in shader!" << std::endl;
				return;
			}

			auto& uniform = m_Uniforms.at(name);
			SetUniform(uniform, (const void*)&value);
		}

		const ShaderInfo& GetInfo() const { return m_Info; }
		const ShaderInputLayout& GetInputLayout() const { return m_InputLayout; }
	private:
		void Compile(const std::unordered_map<ShaderStage, std::string>& sources);
		void Reflect();
	private:
		uint32 m_ProgramID = 0;
		ShaderInfo m_Info;
		ShaderInputLayout m_InputLayout;

		std::unordered_map<std::string, ShaderUniform> m_Uniforms;
	};

}