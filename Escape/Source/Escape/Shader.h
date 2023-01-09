#pragma once

namespace Escape {

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
		Int, Int2, Int3, Int4
	};

	struct ShaderAttribute
	{
		std::string Name;
		uint32 Index;
		uint32 Size;
		uint32 Offset;
		uint32 ComponentCount;
		ShaderDataType Type;
	};

	struct ShaderInputLayout
	{
		uint32 Stride;
		std::vector<ShaderAttribute> Attributes;

		std::vector<ShaderAttribute>::iterator begin() { return Attributes.begin(); }
		std::vector<ShaderAttribute>::iterator end() { return Attributes.end(); }
		std::vector<ShaderAttribute>::const_iterator begin() const { return Attributes.cbegin(); }
		std::vector<ShaderAttribute>::const_iterator end() const { return Attributes.cend(); }
	};

	class Shader : public ReferenceCounted
	{
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

		const ShaderInputLayout& GetInputLayout() const { return m_InputLayout; }
	private:
		void Compile(const std::unordered_map<ShaderStage, std::string>& sources);
		void Reflect();
	private:
		uint32 m_ProgramID = 0;
		ShaderInputLayout m_InputLayout;
	};

}