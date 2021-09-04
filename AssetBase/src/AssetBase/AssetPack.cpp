#include "pch.h"
#include "AssetPack.h"

#include "yaml-cpp/yaml.h"

#include "Resources/Texture2DRes.h"
#include "Resources/ShaderRes.h"
#include "Resources/TextureAtlasRes.h"
#include "Resources/FontLoader.h"
#include "Resources/AudioRes.h"
#include "Resources/LocaleRes.h"
#include "Resources/ModelRes.h"

#include "Util/OptionalField.h"
#include "AssetBase.h"

AssetPack::AssetPack(const std::filesystem::path& path, const std::filesystem::path& models_path, bool debug_mode)
{
	namespace fb = flatbuffers;
	namespace fs = std::filesystem;

	std::cout << "Building from resource definition file " << path.string() << "\n";
		
	YAML::Node resources = YAML::LoadFile(path.string());
	if (!resources || !resources.IsMap())
		throw "Invalid YAML file.";

	fs::remove_all("./.lab-cache/shaders/");

	std::vector<fb::Offset<Assets::Texture2D>> textures;
	std::vector<fb::Offset<Assets::Shader>> shaders;
	std::vector<fb::Offset<Assets::TextureAtlas>> atlases;
	std::vector<fb::Offset<Assets::Font>> fonts;
	std::vector<fb::Offset<Assets::Audio>> audio;
	std::vector<fb::Offset<Assets::Locale>> locales;

	for (const std::pair<YAML::Node, YAML::Node>& resource : resources)
	{
		const std::string& name = resource.first.Scalar();

		std::string type = GetOptionalString(resource.second, "type");
		std::cout << "Loading " << type << " " << name << " ... ";

		if (type == "texture2d")
			textures.push_back(SerializeTexture2D(m_Builder, path, name, resource.second));
		else if (type == "shader")
			shaders.push_back(SerializeShader(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "texture_atlas")
			atlases.push_back(SerializeTextureAtlas(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "font")
			fonts.push_back(AssetBase::Get()->GetFontSerializer().Serialize(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "audio")
			audio.push_back(SerializeAudio(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "locale")
			locales.push_back(SerializeLocale(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "model")
		{
			fb::FlatBufferBuilder model_builder;
			fb::Offset<Assets::Model> model = SerializeModel(model_builder, path, name, resource.second);
			model_builder.Finish(model, "LMOD");

			fs::create_directories(models_path);
			fs::path model_filename = name + ".lmodel";
			std::ofstream model_file(models_path / model_filename, std::ios::out | std::ios::trunc | std::ios::binary);
			model_file.write((const char*)model_builder.GetBufferPointer(), model_builder.GetSize());
		}
		else
		{
			std::cout << "Resource " << name << ": invalid type "<< type << "\n";
			continue;
		}

		std::cout << "done.         \n";
	}

	auto bundle = Assets::CreateAssetBundleDirect(m_Builder, &textures, &shaders, &atlases, &fonts, &audio, &locales);
	m_Builder.Finish(bundle, "LPCK");
}

std::ostream& operator<<(std::ostream& os, const AssetPack& asset_pack)
{
	return os.write((const char*)asset_pack.GetBufferPointer(), asset_pack.GetBufferSize());
}

