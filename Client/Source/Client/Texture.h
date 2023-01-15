#pragma once

#include <filesystem>

namespace esc {

	struct TextureInfo
	{
		std::string Filepath;

		int32 Width = 1;
		int32 Height = 1;
		int32 Channels = 4;
		uint8* Data = nullptr;

		std::string GetName() const
		{
			std::filesystem::path path = Filepath;
			return path.filename().stem().string();
		}
	};

	class Texture : public ReferenceCounted
	{
	public:
		Texture(const TextureInfo& info);
		virtual ~Texture();

		void Bind(uint32 slot = 0) const;
		void Unbind(uint32 slot = 0) const;

		const TextureInfo& GetInfo() const { return m_Info; }
	private:
		uint32 m_TextureID = 0;
		TextureInfo m_Info;
	};

}