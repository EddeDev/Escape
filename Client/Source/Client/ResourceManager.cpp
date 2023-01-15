#include "ClientPCH.h"
#include "ResourceManager.h"

#include "EscapeGame.h"
#include "FileUtils.h"

#include <filesystem>

#include <zlib.h>

namespace esc {

	namespace Utils {

		static bool EndsWith(const std::string& value, const std::string& ending)
		{
			if (ending.size() > value.size())
				return false;

			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

	}

	static constexpr uint64 s_HeaderIdentifier = 0x9b4651a2;

	static const char* s_ResourcePackFilepath = "Resources.esc";
	static const char* s_TextureFileExtension = ".tga";

	struct ResourcePackHeader
	{
		uint64 HeaderIdentifier;
		uint32 NumShaders;
		uint32 NumTextures;
	};

	struct ShaderData
	{
		uint64 VertexShaderSourceSize;
		uint64 FragmentShaderSourceSize;
		uint64 NameSize;
	};

	struct TextureData
	{
		int32 Width;
		int32 Height;
		int32 Channels;
		uint64 NameSize;
	};

	void ResourceManager::Init()
	{
		std::unordered_map<std::string, ShaderInfo> shaderInfos;
		std::unordered_map<std::string, TextureInfo> textureInfos;

#ifdef ESCAPE_BUILD_SHIPPING
		gzFile file = gzopen(s_ResourcePackFilepath, "rb");

		ResourcePackHeader header;
		gzread(file, &header, sizeof(ResourcePackHeader));
		if (header.HeaderIdentifier != s_HeaderIdentifier)
		{
			std::cerr << "Invalid file!" << std::endl;

			EscapeGame::Get().Close();
			return;
		}

		std::vector<ShaderData> shaderDataHeaders(header.NumShaders);
		std::vector<std::string> shaderNames(header.NumShaders);
		std::vector<std::string> vertexShaderSources(header.NumShaders);
		std::vector<std::string> fragmentShaderSources(header.NumShaders);

		for (uint32 i = 0; i < header.NumShaders; i++)
		{
			auto& data = shaderDataHeaders[i];
			gzread(file, &data, sizeof(ShaderData));

			auto& name = shaderNames[i];
			name.resize(data.NameSize);
			gzread(file, name.data(), static_cast<uint32>(name.size()));

			auto& vertexShaderSource = vertexShaderSources[i];
			vertexShaderSource.resize(data.VertexShaderSourceSize);
			gzread(file, vertexShaderSource.data(), static_cast<uint32>(vertexShaderSource.size()));

			auto& fragmentShaderSource = fragmentShaderSources[i];
			fragmentShaderSource.resize(data.FragmentShaderSourceSize);
			gzread(file, fragmentShaderSource.data(), static_cast<uint32>(fragmentShaderSource.size()));
		}

		std::vector<TextureData> textureDataHeaders(header.NumTextures);
		std::vector<std::string> textureNames(header.NumTextures);
		std::vector<std::vector<uint8>> textureDataStorage(header.NumTextures);

		for (uint32 i = 0; i < header.NumTextures; i++)
		{
			auto& data = textureDataHeaders[i];
			gzread(file, &data, sizeof(TextureData));

			auto& name = textureNames[i];
			name.resize(data.NameSize);
			gzread(file, name.data(), static_cast<uint32>(name.size()));

			auto& binaryData = textureDataStorage[i];
			size_t dataSize = data.Width * data.Height * data.Channels;
			binaryData.resize(dataSize);
			gzread(file, binaryData.data(), static_cast<uint32>(binaryData.size()));
		}

		gzclose(file);

		for (uint32 i = 0; i < header.NumShaders; i++)
		{
			std::string shaderName = shaderNames[i];

			ShaderInfo info;
			info.VertexShaderSource = vertexShaderSources[i];
			info.FragmentShaderSource = fragmentShaderSources[i];

			shaderInfos[shaderName] = info;
		}

		for (uint32 i = 0; i < header.NumTextures; i++)
		{
			std::string textureName = textureNames[i];

			TextureInfo info;
			info.Width = textureDataHeaders[i].Width;
			info.Height = textureDataHeaders[i].Height;
			info.Channels = textureDataHeaders[i].Channels;
			info.Data = textureDataStorage[i].data();
			info.Filepath = "Resources/Textures/" + textureName + s_TextureFileExtension;

			textureInfos[textureName] = info;
		}

#else
		for (auto& dirEntry : std::filesystem::recursive_directory_iterator("Resources"))
		{
			const auto& path = dirEntry.path();

			if (path.extension() == ".glsl")
			{
				std::string filename = path.filename().stem().string();

				std::string shaderName = filename.substr(0, filename.find_last_of('_'));

				if (Utils::EndsWith(filename, "_Vertex"))
				{
					if (shaderInfos.find(shaderName) != shaderInfos.end())
					{
						shaderInfos.at(shaderName).VertexShaderSource = FileUtils::ReadFile(path.string());
					}
					else
					{
						ShaderInfo info;
						info.VertexShaderSource = FileUtils::ReadFile(path.string());

						shaderInfos[shaderName] = info;
					}
				}
				else if (Utils::EndsWith(filename, "_Fragment"))
				{
					if (shaderInfos.find(shaderName) != shaderInfos.end())
					{
						shaderInfos.at(shaderName).FragmentShaderSource = FileUtils::ReadFile(path.string());
					}
					else
					{
						ShaderInfo info;
						info.FragmentShaderSource = FileUtils::ReadFile(path.string());

						shaderInfos[shaderName] = info;
					}
				}
			}
			else if (path.extension() == s_TextureFileExtension)
			{
				std::string textureName = path.filename().stem().string();

				TextureInfo info;
				info.Filepath = path.string();

				textureInfos[textureName] = info;
			}
		}
#endif

		for (auto& [shaderName, shaderInfo] : shaderInfos)
			s_Shaders[shaderName] = Ref<Shader>::Create(shaderInfo);

		for (auto& [textureName, textureInfo] : textureInfos)
			s_Textures[textureName] = Ref<Texture>::Create(textureInfo);
	}

	void ResourceManager::Shutdown()
	{
#ifndef ESCAPE_BUILD_SHIPPING
		gzFile file = gzopen(s_ResourcePackFilepath, "wb");

		ResourcePackHeader header;
		header.HeaderIdentifier = s_HeaderIdentifier;
		header.NumShaders = static_cast<uint32>(s_Shaders.size());
		header.NumTextures = static_cast<uint32>(s_Textures.size());

		gzwrite(file, &header, sizeof(ResourcePackHeader));

		for (auto& [shaderName, shader] : s_Shaders)
		{
			auto& info = shader->GetInfo();

			ShaderData data;
			data.VertexShaderSourceSize = static_cast<uint64>(info.VertexShaderSource.size());
			data.FragmentShaderSourceSize = static_cast<uint64>(info.FragmentShaderSource.size());
			data.NameSize = shaderName.size();

			gzwrite(file, &data, sizeof(ShaderData));
			gzwrite(file, shaderName.data(), static_cast<uint32>(shaderName.size()));
			gzwrite(file, info.VertexShaderSource.c_str(), static_cast<uint32>(info.VertexShaderSource.size()));
			gzwrite(file, info.FragmentShaderSource.c_str(), static_cast<uint32>(info.FragmentShaderSource.size()));
		}

		for (auto& [textureName, texture] : s_Textures)
		{
			auto& info = texture->GetInfo();

			TextureData data;
			data.Width = info.Width;
			data.Height = info.Height;
			data.Channels = info.Channels;
			data.NameSize = textureName.size();

			if (!info.Data)
			{
				std::cout << "Texture is null!" << std::endl;
				continue;
			}

			uint32 dataSize = data.Width * data.Height * data.Channels;

			gzwrite(file, &data, sizeof(TextureData));
			gzwrite(file, textureName.data(), static_cast<uint32>(textureName.size()));
			gzwrite(file, info.Data, dataSize);
		}
		
		gzclose(file);
#endif
	}

	Ref<Shader> ResourceManager::GetShader(const std::string& name)
	{
		if (s_Shaders.find(name) != s_Shaders.end())
			return s_Shaders.at(name);

		return nullptr;
	}

	Ref<Texture> ResourceManager::GetTexture(const std::string& name)
	{
		if (s_Textures.find(name) != s_Textures.end())
			return s_Textures.at(name);

		return nullptr;
	}

}