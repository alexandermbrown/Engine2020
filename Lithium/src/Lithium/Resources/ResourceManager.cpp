#include "lipch.h"
#include "ResourceManager.h"

#include "Loaders/ShaderLoader.h"
#include "Loaders/TextureAtlasLoader.h"
#include "Loaders/FontLoader.h"
#include "Loaders/LocaleLoader.h"

#include "Lithium/Core/Exceptions.h"
#include "Lithium/Renderer/GraphicsFactory.h"

#include <fstream>

namespace Li
{
	Unique<ResourceManager::ResourceData> ResourceManager::s_Data = nullptr;

	void ResourceManager::Init()
	{
		LI_CORE_ASSERT(s_Data == nullptr, "ResourceManager already instantiated");
		s_Data = MakeUnique<ResourceData>();

		// SETUP WHITE TEXTURE
		uint32_t data = 0xffffffff;
		Ref<Texture2D> texture_white = GraphicsFactory::Get()->MakeTexture2D(1, 1, 4, &data, TextureProps{});

		// SETUP SCENE FLAT COLOR TEXTURE ALTAS
		Ref<TextureAtlas> texture_white_atlas = MakeRef<TextureAtlas>(TextureAtlas(texture_white, {
			{ "texture_white", glm::vec4(0.5f) }
		}));

		s_Data->Textures["texture_white"] = texture_white;
		s_Data->TextureAtlases["texture_white_atlas"] = texture_white_atlas;
	}

	void ResourceManager::Shutdown()
	{
		if (s_Data->LoadData.Buffer)
		{
			Log::CoreWarn("Load data buffer not freed.");
			delete[] s_Data->LoadData.Buffer;
		}
		s_Data.reset();
	}

	void ResourceManager::Load(const std::string& lpack_file_path)
	{
		Log::CoreInfo("Loading asset pack {}...", lpack_file_path);

		std::ifstream in_file(lpack_file_path, std::ios::in | std::ios::binary);
		if (!in_file.good())
			throw AssetPackError("Error opening asset pack " + lpack_file_path);

		in_file.seekg(0, std::ios::end);
		long filesize = (long)in_file.tellg();
		in_file.seekg(0, std::ios::beg);
		std::vector<uint8_t> buffer(filesize);
		in_file.read((char*)buffer.data(), filesize);
		in_file.close();

		const Assets::AssetBundle* asset_bundle = flatbuffers::GetRoot<Assets::AssetBundle>(buffer.data());
		flatbuffers::Verifier verifier(buffer.data(), filesize);
		if (!asset_bundle->Verify(verifier))
			throw AssetPackError("Corrupt asset pack " + lpack_file_path + ". Verify or reinstall game files.");

		for (const Assets::Texture2D* texture : *asset_bundle->textures())
		{
			const auto* data = texture->data();
			TextureProps props{
				(WrapType)texture->wrap_s(), (WrapType)texture->wrap_t(),
				(FilterType)texture->min_filter(), (FilterType)texture->mag_filter()
			};
			s_Data->Textures[texture->name()->c_str()] = GraphicsFactory::Get()->MakeTexture2DFromEncoded((size_t)data->size(), data->data(), 4, props);
		}

		for (const Assets::Shader* shader : *asset_bundle->shaders())
		{
			s_Data->Shaders[shader->name()->c_str()] = Loaders::LoadShader(shader);
		}

		for (const Assets::TextureAtlas* atlas : *asset_bundle->atlases())
		{
			s_Data->TextureAtlases[atlas->name()->c_str()] = Loaders::LoadTextureAtlas(atlas);
		}

		for (const Assets::Font* font : *asset_bundle->fonts())
		{
			s_Data->Fonts[font->name()->c_str()] = Loaders::LoadFont(font);
		}

		//for (const Assets::Audio* audio : *asset_bundle->audio())
		//{
		//	const auto* data = audio->data();
		//	s_Data->Audio[audio->name()->c_str()] = AudioBuffer::FromOggMemory(data->data(), (uint32_t)data->size());
		//}

		for (const Assets::Locale* locale : *asset_bundle->locales())
		{
			Localization::AddLocale(Loaders::LoadLocale(locale));
		}
	}

	void ResourceManager::BeginStaggeredLoad(const std::string& lab_file_path)
	{
		Log::CoreInfo("Loading asset base {} staggered...", lab_file_path);
		std::ifstream in_file(lab_file_path, std::ios::in | std::ios::binary);

		if (!in_file.good())
			throw std::runtime_error("Error opening asset base " + lab_file_path);

		in_file.seekg(0, std::ios::end);
		long filesize = (long)in_file.tellg();
		in_file.seekg(0, std::ios::beg);
		s_Data->LoadData.Buffer = new uint8_t[filesize];
		in_file.read((char*)s_Data->LoadData.Buffer, filesize);
		in_file.close();

		s_Data->LoadData.Bundle = flatbuffers::GetRoot<Assets::AssetBundle>(s_Data->LoadData.Buffer);
		flatbuffers::Verifier verifier(s_Data->LoadData.Buffer, filesize);
		if (!s_Data->LoadData.Bundle->Verify(verifier))
			throw std::runtime_error("Corrupt asset base " + lab_file_path + ". Verify or reinstall game files.");
		
		s_Data->LoadData.TextureIt = s_Data->LoadData.Bundle->textures()->begin();
		s_Data->LoadData.ShaderIt = s_Data->LoadData.Bundle->shaders()->begin();
		s_Data->LoadData.TextureAtlasIt = s_Data->LoadData.Bundle->atlases()->begin();
		s_Data->LoadData.FontIt = s_Data->LoadData.Bundle->fonts()->begin();
		s_Data->LoadData.AudioIt = s_Data->LoadData.Bundle->audio()->begin();
		s_Data->LoadData.LocaleIt = s_Data->LoadData.Bundle->locales()->begin();
	}

	bool ResourceManager::UpdateStaggeredLoad()
	{
		LI_CORE_ASSERT(s_Data->LoadData.Buffer && s_Data->LoadData.Bundle, "No assets to load.");

		if (s_Data->LoadData.TextureIt != s_Data->LoadData.Bundle->textures()->end())
		{
			const Assets::Texture2D* texture = *s_Data->LoadData.TextureIt;
			const auto* data = texture->data();

			TextureProps props{
				(WrapType)texture->wrap_s(), (WrapType)texture->wrap_t(),
				(FilterType)texture->min_filter(), (FilterType)texture->mag_filter()
			};
			s_Data->Textures[texture->name()->c_str()] = GraphicsFactory::Get()->MakeTexture2DFromEncoded((size_t)data->size(), data->data(), 4, props);

			s_Data->LoadData.TextureIt++;
		}
		else if(s_Data->LoadData.ShaderIt != s_Data->LoadData.Bundle->shaders()->end())
		{
			const Assets::Shader* shader = *s_Data->LoadData.ShaderIt;
			s_Data->Shaders[shader->name()->c_str()] = Loaders::LoadShader(shader);
			std::string jeff(shader->name()->string_view());
			s_Data->LoadData.ShaderIt++;
		}
		else if (s_Data->LoadData.TextureAtlasIt != s_Data->LoadData.Bundle->atlases()->end())
		{
			const Assets::TextureAtlas* atlas = *s_Data->LoadData.TextureAtlasIt;
			s_Data->TextureAtlases[atlas->name()->c_str()] = Loaders::LoadTextureAtlas(atlas);

			s_Data->LoadData.TextureAtlasIt++;
		}
		else if (s_Data->LoadData.FontIt != s_Data->LoadData.Bundle->fonts()->end())
		{
			const Assets::Font* font = *s_Data->LoadData.FontIt;
			s_Data->Fonts[font->name()->c_str()] = Loaders::LoadFont(font);

			s_Data->LoadData.FontIt++;
		}
		//else if (s_Data->LoadData.AudioIt != s_Data->LoadData.Bundle->audio()->end())
		//{
		//	const Assets::Audio* audio = *s_Data->LoadData.AudioIt;
		//	const auto* data = audio->data();
		//	s_Data->Audio[audio->name()->c_str()] = AudioBuffer::FromOggMemory(data->data(), (uint32_t)data->size());

		//	s_Data->LoadData.AudioIt++;
		//}
		else if (s_Data->LoadData.LocaleIt != s_Data->LoadData.Bundle->locales()->end())
		{
			const Assets::Locale* locale = *s_Data->LoadData.LocaleIt;
			Localization::AddLocale(Loaders::LoadLocale(locale));

			s_Data->LoadData.LocaleIt++;
		}
		else
		{
			delete[] s_Data->LoadData.Buffer;
			s_Data->LoadData.Buffer = nullptr;
			s_Data->LoadData.Bundle = nullptr;
			return false;
		}

		return true;
	}

	void ResourceManager::PrintInfo()
	{
		Log::CoreInfo("--------------------------------");
		Log::CoreInfo("           Asset Info");
		Log::CoreInfo("--------------------------------");
		Log::CoreInfo("    # Textures        | {}", s_Data->Textures.size());
		Log::CoreInfo("    # Shaders         | {}", s_Data->Shaders.size());
		Log::CoreInfo("    # TextureAtlases  | {}", s_Data->TextureAtlases.size());
		Log::CoreInfo("    # Fonts           | {}", s_Data->Fonts.size());
		//Log::CoreInfo("    # Audio           | {}", s_Data->Audio.size());
		Log::CoreInfo("    # Locales         | {}", Localization::GetLocaleCount());
		Log::CoreInfo("--------------------------------");
	}													  
}
