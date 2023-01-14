#include "ClientPCH.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace esc {

	Texture::Texture(const TextureInfo& info)
	{
		m_Info.Filepath = info.Filepath;
		m_Info.Width = info.Width;
		m_Info.Height = info.Height;
		m_Info.Channels = info.Channels;

		if (info.Data)
		{
			size_t dataSize = info.Width * info.Height * info.Channels;

			m_Info.Data = new uint8[dataSize];
			memcpy(m_Info.Data, info.Data, dataSize);
		}
		else
		{
			uint8* data = stbi_load(m_Info.Filepath.c_str(), &m_Info.Width, &m_Info.Height, &m_Info.Channels, STBI_default);

			if (data)
			{
				size_t dataSize = m_Info.Width * m_Info.Height * m_Info.Channels;

				m_Info.Data = new uint8[dataSize];
				memcpy(m_Info.Data, data, dataSize);
			}
			else
			{
				std::cerr << "Could not load texture '" << m_Info.Filepath << "'" << std::endl;
				std::cerr << stbi_failure_reason() << std::endl;
				__debugbreak();
				return;
			}
		}

		uint32 format = 0;
		uint32 internalFormat = 0;
		if (m_Info.Channels == 3)
		{
			format = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else
		{
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Info.Width, m_Info.Height);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Info.Width, m_Info.Height, format, GL_UNSIGNED_BYTE, static_cast<const void*>(m_Info.Data));
		glGenerateTextureMipmap(m_TextureID);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	Texture::~Texture()
	{
		delete[] m_Info.Data;

		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void Texture::Unbind(uint32 slot) const
	{
		glBindTextureUnit(slot, 0);
	}

}