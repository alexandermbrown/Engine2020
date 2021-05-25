#pragma once

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"
#include "yaml-cpp/yaml.h"

#define MSDFGEN_USE_CPP11
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#undef MSDFGEN_USE_CPP11

#include <filesystem>

class FontSerializer
{
public:
	FontSerializer();
	~FontSerializer();

	flatbuffers::Offset<Assets::Font> Serialize(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode);

private:
	flatbuffers::Offset<flatbuffers::Vector<uint8_t>> LoadTTF(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& path);
	bool TryLoadCache(flatbuffers::FlatBufferBuilder& builder, const std::string& name, const std::filesystem::path& cache_path,
		int16_t glyph_width, float* out_dist_gradient,
		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::FontImage>>>* out_images,
		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>>* out_glyphs);

	void GenerateMSDF(flatbuffers::FlatBufferBuilder& builder, const std::string& name, const std::filesystem::path& cache_path,
		int16_t glyph_width, const std::filesystem::path& font_path, float* out_dist_gradient,
		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::FontImage>>>* out_images,
		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>>* out_glyphs);

	msdfgen::FreetypeHandle* ft = nullptr;
};
