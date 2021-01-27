#include "lipch.h"
#include "ResourceManager.h"

#include "Loaders/TextureAtlasLoader.h"
#include "Loaders/FontLoader.h"
#include "Loaders/LocaleLoader.h"

#include <fstream>

namespace Li
{
	Unique<ResourceManager::ResourceData> ResourceManager::s_Data = MakeUnique<ResourceData>();

	void ResourceManager::Load(const std::string& labFilePath)
	{
		Log::CoreInfo("Loading asset base {}...", labFilePath);
		std::ifstream inFile(labFilePath, std::ios::in | std::ios::binary);

		if (!inFile.good()) {
			Log::CoreError("Error opening lithium asset base {}", labFilePath);
			return;
		}

		inFile.seekg(0, std::ios::end);
		long filesize = (long)inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		uint8_t* buffer = new uint8_t[filesize];
		inFile.read((char*)buffer, filesize);
		inFile.close();

		const Assets::AssetBundle* asset_bundle = flatbuffers::GetRoot<Assets::AssetBundle>(buffer);
		if (!asset_bundle->Verify(flatbuffers::Verifier(buffer, filesize))) 
		{
			Log::CoreError("Invalid lithium asset base {}", labFilePath);
			return;
		}

		for (const Assets::Texture2D* texture : *asset_bundle->textures())
		{
			const auto* data = texture->data();
			s_Data->Textures[texture->name()->str()] = Texture2D::Create((size_t)data->size(), data->data(), 4,
				(WrapType)texture->wrap_s(), (WrapType)texture->wrap_t(), (FilterType)texture->min_filter(), (FilterType)texture->mag_filter());
		}

		for (const Assets::Shader* shader : *asset_bundle->shaders())
		{
			const auto* hlsl_vs = shader->hlsl_vs();
			const auto* hlsl_ps = shader->hlsl_ps();
			std::string name = shader->name()->str();
			s_Data->Shaders[name] = Shader::Create(name, shader->glsl()->str(), hlsl_vs->data(), hlsl_vs->size(), hlsl_ps->data(), hlsl_ps->size());
		}

		for (const Assets::TextureAtlas* atlas : *asset_bundle->atlases())
		{
			s_Data->TextureAtlases[atlas->name()->c_str()] = Loaders::LoadTextureAtlas(atlas);
		}

		for (const Assets::Font* font : *asset_bundle->fonts())
		{
			s_Data->Fonts[font->name()->str()] = Loaders::LoadFont(font);
		}

		for (const Assets::Audio* audio : *asset_bundle->audio())
		{
			const auto* data = audio->data();
			s_Data->Audio[audio->name()->str()] = AudioBuffer::FromOggMemory(data->data(), (uint32_t)data->size());
		}

		for (const Assets::Locale* locale : *asset_bundle->locales())
		{
			Localization::AddLocale(Loaders::LoadLocale(locale));
		}

		delete[] buffer;
	}

	void ResourceManager::BeginStaggeredLoad(const std::string& labFilePath)
	{
		Log::CoreInfo("Loading asset base {} staggered...", labFilePath);
		std::ifstream inFile(labFilePath, std::ios::in | std::ios::binary);

		if (!inFile.good()) {
			Log::CoreError("Error opening lithium asset base {}", labFilePath);
			return;
		}

		inFile.seekg(0, std::ios::end);
		long filesize = (long)inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		s_Data->LoadData.Buffer = new uint8_t[filesize];
		inFile.read((char*)s_Data->LoadData.Buffer, filesize);
		inFile.close();

		s_Data->LoadData.Bundle = flatbuffers::GetRoot<Assets::AssetBundle>(s_Data->LoadData.Buffer);
		if (!s_Data->LoadData.Bundle->Verify(flatbuffers::Verifier(s_Data->LoadData.Buffer, filesize)))
		{
			Log::CoreError("Invalid lithium asset base {}", labFilePath);
			return;
		}
		
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
			s_Data->Textures[texture->name()->str()] = Texture2D::Create((size_t)data->size(), data->data(), 4,
				(WrapType)texture->wrap_s(), (WrapType)texture->wrap_t(), (FilterType)texture->min_filter(), (FilterType)texture->mag_filter());

			s_Data->LoadData.TextureIt++;
		}
		else if(s_Data->LoadData.ShaderIt != s_Data->LoadData.Bundle->shaders()->end())
		{
			const Assets::Shader* shader = *s_Data->LoadData.ShaderIt;
			const auto* hlsl_vs = shader->hlsl_vs();
			const auto* hlsl_ps = shader->hlsl_ps();
			std::string name = shader->name()->str();
			s_Data->Shaders[name] = Shader::Create(name, shader->glsl()->str(), hlsl_vs->data(), hlsl_vs->size(), hlsl_ps->data(), hlsl_ps->size());

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
			s_Data->Fonts[font->name()->str()] = Loaders::LoadFont(font);

			s_Data->LoadData.FontIt++;
		}
		else if (s_Data->LoadData.AudioIt != s_Data->LoadData.Bundle->audio()->end())
		{
			const Assets::Audio* audio = *s_Data->LoadData.AudioIt;
			const auto* data = audio->data();
			s_Data->Audio[audio->name()->str()] = AudioBuffer::FromOggMemory(data->data(), (uint32_t)data->size());

			s_Data->LoadData.AudioIt++;
		}
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

	void ResourceManager::Shutdown()
	{
		if (s_Data->LoadData.Buffer)
		{
			Log::CoreWarn("Load data buffer not freed.");
			delete[] s_Data->LoadData.Buffer;
		}
		s_Data.reset();
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
		Log::CoreInfo("    # Audio           | {}", s_Data->Audio.size());
		Log::CoreInfo("    # Locales         | {}", Localization::GetLocaleCount());
		Log::CoreInfo("--------------------------------");
	}
}
