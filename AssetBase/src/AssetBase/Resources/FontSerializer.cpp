#include "pch.h"
#include "FontSerializer.h"

#include "AssetBase/Util/Convert.h"
#include "AssetBase/Util/OptionalField.h"

#include <math.h>

namespace fb = flatbuffers;
namespace fs = std::filesystem;
constexpr int MaxTextureWidth = 2048;
constexpr int MaxTextureWidthSquared = MaxTextureWidth * MaxTextureWidth;

FontSerializer::FontSerializer()
	: ft(msdfgen::initializeFreetype())
{
	if (!ft)
		throw "Failed to initialize freetype.";
}

FontSerializer::~FontSerializer()
{
	msdfgen::deinitializeFreetype(ft);
	ft = nullptr;
}

fb::Offset<Assets::Font> FontSerializer::Serialize(fb::FlatBufferBuilder& builder, const fs::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode)
{
	fb::Offset<fb::String> name_offset = builder.CreateString(name);

	fs::path font_path = base_path.parent_path() / GetOptionalString(atlas, "path");

	int16_t glyph_width = Convert::StringToInt(GetOptionalString(atlas, "glyph_width"), "glyph_width");

	fb::Offset<fb::Vector<fb::Offset<Assets::FontImage>>> images_offset = NULL;
	fb::Offset<fb::Vector<fb::Offset<Assets::GlyphEntry>>> glyphs_offset = NULL;
	fb::Offset<fb::Vector<uint8_t>> ttf = LoadTTF(builder, font_path);
	float distance_gradient;

	fs::path cache_path = "./.lab-cache/fonts/" + name + ".fontcache";
	bool use_cache = TryLoadCache(builder, name, cache_path, glyph_width, &distance_gradient, &images_offset, &glyphs_offset);

	if (!use_cache)
		GenerateMSDF(builder, name, cache_path, glyph_width, font_path, &distance_gradient, &images_offset, &glyphs_offset);

	return Assets::CreateFont(builder, name_offset, glyph_width, distance_gradient, glyphs_offset, images_offset, ttf);
}

fb::Offset<fb::Vector<uint8_t>> FontSerializer::LoadTTF(fb::FlatBufferBuilder& builder, const fs::path& path)
{
	std::ifstream ttf_file(path, std::ios::in | std::ios::binary);
	if (!ttf_file.is_open())
		throw "Failed to open ttf file.\n";

	ttf_file.ignore(std::numeric_limits<std::streamsize>::max());
	size_t file_size = ttf_file.gcount();
	ttf_file.clear();
	ttf_file.seekg(0, std::ios_base::beg);

	uint8_t* file_data;
	fb::Offset<fb::Vector<uint8_t>> ttf_offset = builder.CreateUninitializedVector(file_size, &file_data);

	ttf_file.read((char*)file_data, file_size);
	ttf_file.close();

	return ttf_offset;
}

bool FontSerializer::TryLoadCache(fb::FlatBufferBuilder& builder, const std::string& name,
	const fs::path& cache_path, int16_t glyph_width, float* out_dist_gradient,
	fb::Offset<fb::Vector<fb::Offset<Assets::FontImage>>>* out_images,
	fb::Offset<fb::Vector<fb::Offset<Assets::GlyphEntry>>>* out_glyphs)
{
	if (!fs::exists(cache_path))
		return false;
	
	std::ifstream cache_file(cache_path, std::ios::in | std::ios::binary);

	cache_file.seekg(0, std::ios::end);
	int file_size = (int)cache_file.tellg();
	cache_file.seekg(0, std::ios::beg);
	std::vector<uint8_t> buffer(file_size);
	cache_file.read((char*)buffer.data(), file_size);
	cache_file.close();

	const Assets::Font* font = fb::GetRoot<Assets::Font>(buffer.data());

	if (!font->Verify(fb::Verifier(buffer.data(), file_size)))
		return false;
	
	if (name != font->name()->c_str() || glyph_width != font->glyph_width())
		return false;

	std::vector<fb::Offset<Assets::FontImage>> images_vec;
	std::vector<fb::Offset<Assets::GlyphEntry>> glyphs_vec;

	*out_dist_gradient = font->distance_gradient();

	const auto* images_vector = font->images();
	for (const Assets::FontImage* cached_image : *images_vector)
	{
		const auto* image_vector = cached_image->image();
		fb::Offset<fb::Vector<uint8_t>> image = builder.CreateVector(image_vector->data(), image_vector->size());
		images_vec.push_back(Assets::CreateFontImage(builder, cached_image->width(), image));
	}

	const auto* glyphs_vector = font->glyphs();
	for (const Assets::GlyphEntry* cached_glyph : *glyphs_vector)
	{
		glyphs_vec.push_back(Assets::CreateGlyphEntry(builder, cached_glyph->codepoint(), cached_glyph->texture_offset()));
	}

	*out_images = builder.CreateVector(images_vec);
	*out_glyphs = builder.CreateVector(glyphs_vec);
	
	return true;
}

void FontSerializer::GenerateMSDF(fb::FlatBufferBuilder& builder, const std::string& name, const fs::path& cache_path,
	int16_t glyph_width, const fs::path& font_path, float* out_dist_gradient,
	fb::Offset<fb::Vector<fb::Offset<Assets::FontImage>>>* out_images,
	fb::Offset<fb::Vector<fb::Offset<Assets::GlyphEntry>>>* out_glyphs)
{
	msdfgen::FontHandle* font = msdfgen::loadFont(ft, font_path.u8string().c_str());
	if (!font)
		throw "Failed to load font.";

	int num_glyphs = msdfgen::getGlyphCount(font);
	int num_glyphs_left = num_glyphs;
	
	int glyph_width_squared = (int)glyph_width * (int)glyph_width;
	int max_glyphs_per_texture = MaxTextureWidthSquared / glyph_width_squared;

	if (num_glyphs > 8 * max_glyphs_per_texture)
		throw std::runtime_error("Cannot fit glyphs into textures");

	int count = 0;
	std::cout << "Generating " << num_glyphs << " glyphs.\n";
	std::cout << "[" << count << " / " << num_glyphs << "]\r";

	fb::FlatBufferBuilder cache_builder;

	std::vector<fb::Offset<Assets::GlyphEntry>> glyphs, cache_glyphs;
	std::vector<fb::Offset<Assets::FontImage>> images, cache_images;

	msdfgen::FontMetrics metrics;
	msdfgen::getFontMetrics(metrics, font);
	*out_dist_gradient = static_cast<float>(1.953125 / metrics.emSize);

	// Generate multiple textures to fit all glyphs
	// All 2048x2048 except last which is just big enough to fit leftover glyphs, 2^n.
	uint32_t codepoint;
	unsigned long charcode = msdfgen::getFirstChar(font, &codepoint);


	while (num_glyphs_left > 0)
	{
		int num_in_image = std::min(max_glyphs_per_texture, num_glyphs_left);

		double required_width = sqrt(num_in_image * glyph_width_squared);
		int image_width = static_cast<long>(pow(2, ceil(log2(required_width))));
		
		msdfgen::Bitmap<float, 3> atlas(image_width, image_width);

		for (int image_count = 0; image_count < num_in_image; image_count++)
		{
			msdfgen::Shape shape;
			if (!msdfgen::loadGlyph(shape, font, charcode))
				throw std::stringstream("Failed to load glyph ") << charcode << " in font " << name << ".\n";

			shape.normalize();
			msdfgen::edgeColoringSimple(shape, 3.0);
			msdfgen::Bitmap<float, 3> msdf(glyph_width, glyph_width);
			msdfgen::generateMSDF(msdf, shape, 4.0, msdfgen::Vector2(0.6f * (float)glyph_width / (float)metrics.emSize), msdfgen::Vector2(4.0, 10.0));

			// Combine images.
			for (int y = 0; y < glyph_width; y++) {
				for (int x = 0; x < glyph_width; x++) {
					for (uint32_t channel = 0; channel < 3; channel++) {
						atlas(
							(glyph_width * image_count) % image_width + x,
							(int)std::floor((float)(glyph_width * image_count) / (float)image_width) * glyph_width + y
						)[channel] = msdf(x, y)[channel];
					}
				}
			}

			Assets::Vec2 texture_offset = {
				((glyph_width * image_count) % image_width) / (float)image_width,
				(std::floor((float)(glyph_width * image_count) / (float)image_width) * glyph_width) / (float)image_width
			};

			uint8_t texture_index = (uint8_t)images.size();
			glyphs.push_back(Assets::CreateGlyphEntry(builder, codepoint, &texture_offset, texture_index));
			cache_glyphs.push_back(Assets::CreateGlyphEntry(cache_builder, codepoint, &texture_offset, texture_index));

			if (count % 10 == 0)
				std::cout << "[" << count << " / " << num_glyphs << "]\r";

			count++;
			charcode = msdfgen::getNextChar(font, charcode, &codepoint);
		}

		std::vector<uint8_t> image_data;
		msdfgen::savePng(atlas, (name + std::to_string(images.size()) + ".png").c_str());
		msdfgen::savePng(atlas, image_data);
		
		images.push_back(Assets::CreateFontImageDirect(builder, image_width, &image_data));
		cache_images.push_back(Assets::CreateFontImageDirect(cache_builder, image_width, &image_data));

		num_glyphs_left -= num_in_image;
	}
	msdfgen::destroyFont(font);

	*out_glyphs = builder.CreateVector(cache_glyphs);
	*out_images = builder.CreateVector(cache_images);

	auto cache_glyphs_offset = cache_builder.CreateVector(cache_glyphs);
	auto cache_images_offset = cache_builder.CreateVector(cache_images);

	auto cache_font = Assets::CreateFont(cache_builder, cache_builder.CreateString(name), glyph_width, *out_dist_gradient, cache_glyphs_offset, cache_images_offset);
	cache_builder.Finish(cache_font, "FONT");

	// Write cache file.
	fs::create_directories(cache_path.parent_path());
	std::ofstream cache_file(cache_path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!cache_file.is_open() || !cache_file.good()) {
		throw "Failed to write cache file!";
	}

	cache_file.write((const char*)cache_builder.GetBufferPointer(), cache_builder.GetSize());
	cache_file.close();
}
