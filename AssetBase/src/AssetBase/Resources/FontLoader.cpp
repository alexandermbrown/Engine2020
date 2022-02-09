#include "pch.h"
#include "FontLoader.h"

#include "AssetBase/Util/Convert.h"
#include "AssetBase/Util/OptionalField.h"

#include "stb_image_write.h"

#include <math.h>
#include <thread>
#include <fstream>
#include <iostream>

namespace fb = flatbuffers;
namespace fs = std::filesystem;
//constexpr int MaxTextureWidth = 2048;
//constexpr int MaxTextureWidthSquared = MaxTextureWidth * MaxTextureWidth;

FontLoader::FontLoader()
{
	FT_Error error = FT_Init_FreeType(&m_FT);
	if (error != 0)
		throw std::runtime_error("Failed to initialize freetype!");
}

FontLoader::~FontLoader()
{
	FT_Done_FreeType(m_FT);
	m_FT = nullptr;
}

fb::Offset<Assets::Font> FontLoader::Serialize(fb::FlatBufferBuilder& builder, const fs::path& base_path, const std::string& name, YAML::Node font, bool debug_mode)
{
	fb::Offset<fb::String> name_offset = builder.CreateString(name);

	fs::path font_path = base_path.parent_path() / GetOptionalString(font, "path");

	int16_t glyph_scale = Convert::StringToInt(GetOptionalString(font, "glyph_scale"), "glyph_scale");

	fb::Offset<fb::Vector<fb::Offset<Assets::FontImage>>> images_offset;
	fb::Offset<fb::Vector<fb::Offset<Assets::GlyphEntry>>> glyphs_offset;
	fb::Offset<fb::Vector<uint8_t>> ttf = LoadTTF(builder, font_path);

	float em_size;
	fs::path cache_path = "./.lab-cache/fonts/" + name + ".fontcache";
	bool use_cache = TryLoadCache(builder, name, cache_path, glyph_scale, &em_size, &images_offset, &glyphs_offset);

	if (!use_cache)
		GenerateMSDF(builder, name, cache_path, glyph_scale, font_path, &em_size, &images_offset, &glyphs_offset);

	return Assets::CreateFont(builder, name_offset, glyph_scale, em_size, glyphs_offset, images_offset, ttf);
}

fb::Offset<fb::Vector<uint8_t>> FontLoader::LoadTTF(fb::FlatBufferBuilder& builder, const fs::path& path)
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

bool FontLoader::TryLoadCache(fb::FlatBufferBuilder& builder, const std::string& name,
	const fs::path& cache_path, int16_t glyph_scale, float* out_em_size,
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

	fb::Verifier verifier(buffer.data(), file_size);
	if (!font->Verify(verifier))
		return false;
	
	if (name != font->name()->c_str() || glyph_scale != font->glyph_scale())
		return false;

	std::vector<fb::Offset<Assets::FontImage>> images_vec;
	std::vector<fb::Offset<Assets::GlyphEntry>> glyphs_vec;

	*out_em_size = font->em_size();

	const auto* images_vector = font->images();
	for (const Assets::FontImage* cached_image : *images_vector)
	{
		const auto* image_vector = cached_image->image();
		fb::Offset<fb::Vector<uint8_t>> image = builder.CreateVector(image_vector->data(), image_vector->size());
		images_vec.push_back(Assets::CreateFontImage(builder, cached_image->width(), image));
	}

	const auto* glyphs_vector = font->glyphs();
	for (const Assets::GlyphEntry* cached_glyph : *glyphs_vector)
		glyphs_vec.push_back(Assets::CreateGlyphEntry(builder, cached_glyph->gindex(), cached_glyph->atlas_bounds(), cached_glyph->plane_bounds(), cached_glyph->texture_index()));

	*out_images = builder.CreateVector(images_vec);
	*out_glyphs = builder.CreateVector(glyphs_vec);
	
	return true;
}

void FontLoader::GenerateMSDF(fb::FlatBufferBuilder& builder, const std::string& name, const fs::path& cache_path,
	int16_t glyph_scale, const fs::path& font_path, float* out_em_size,
	fb::Offset<fb::Vector<fb::Offset<Assets::FontImage>>>* out_images,
	fb::Offset<fb::Vector<fb::Offset<Assets::GlyphEntry>>>* out_glyphs)
{
	FT_Face face;
	FT_New_Face(m_FT, font_path.u8string().c_str(), 0, &face);

	msdfgen::FontHandle* font = msdfgen::adoptFreetypeFont(face);

	msdfgen::FontMetrics metrics;
	msdfgen::getFontMetrics(metrics, font);

	*out_em_size = static_cast<float>(metrics.emSize);

	std::vector<msdf_atlas::GlyphGeometry> glyphs;
	std::vector<FT_UInt> glyph_indices;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(face, &gindex);
	while (gindex != 0)
	{
		msdf_atlas::GlyphGeometry glyph;
		if (glyph.load(font, 1.0, charcode))
		{
			glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3.0, 0);
			glyphs.push_back(std::move(glyph));
			glyph_indices.push_back(gindex);
		}
		else
			std::cout << "Glyph for charcode " << charcode << " missing.";
		
		charcode = FT_Get_Next_Char(face, charcode, &gindex);
	}
	if (glyphs.size() == 0)
		throw std::runtime_error("No glyphs loaded.");

	msdf_atlas::TightAtlasPacker packer;
	packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_SQUARE);
	packer.setPadding(0);
	packer.setScale(glyph_scale / metrics.emSize);
	packer.setPixelRange(2.0);
	packer.setUnitRange(0.0);
	packer.setMiterLimit(1.0);

	if (int remaining = packer.pack(glyphs.data(), (int)glyphs.size()))
	{
		if (remaining < 0)
			throw std::runtime_error("Failed to pack glyphs into atlas.");
		else
			throw std::runtime_error("Could not fit glyphs into atlas.");
	}

	int image_width, image_height;
	packer.getDimensions(image_width, image_height);

	if (image_width != image_height)
		throw std::runtime_error("Image width does not equal height");
	if (image_width > 2048)
		throw std::runtime_error("Image width greater than 2048. Implement multiple atlases.");

	
	msdf_atlas::ImmediateAtlasGenerator<float, MSDFChannels, msdf_atlas::msdfGenerator,
		msdf_atlas::BitmapAtlasStorage<BitmapStorageType, MSDFChannels>> generator(image_width, image_height);

	msdf_atlas::GeneratorAttributes attributes;
	attributes.config.overlapSupport = false;
	attributes.scanlinePass = false;

	int n_threads = std::max((int)std::thread::hardware_concurrency(), 1);
	generator.setAttributes(attributes);
	generator.setThreadCount(n_threads);

	generator.generate(glyphs.data(), (int)glyphs.size());
	msdfgen::BitmapConstRef<BitmapStorageType, MSDFChannels> bitmap = generator.atlasStorage();

	msdfgen::destroyFont(font);
	FT_Done_Face(face);

	fb::FlatBufferBuilder cache_builder;
	std::vector<fb::Offset<Assets::GlyphEntry>> out_glyphs_vec, cache_glyphs_vec;
	std::vector<fb::Offset<Assets::FontImage>> out_images_vec, cache_images_vec;
	// Save glyph bounds.
	auto it = glyph_indices.begin();
	for (const msdf_atlas::GlyphGeometry& glyph : glyphs)
	{
		double left, bottom, right, top;

		glyph.getQuadAtlasBounds(left, bottom, right, top);
		Assets::Vec4 atlas_bounds = { 
			static_cast<float>(left / image_width),
			static_cast<float>(bottom / image_height),
			static_cast<float>(right / image_width),
			static_cast<float>(top / image_height)
		};

		glyph.getQuadPlaneBounds(left, bottom, right, top);
		Assets::Vec4 plane_bounds = {
			static_cast<float>(left),
			static_cast<float>(bottom),
			static_cast<float>(right),
			static_cast<float>(top)
		};

		uint32_t gindex = *it;
		out_glyphs_vec.push_back(Assets::CreateGlyphEntry(builder, gindex, &atlas_bounds, &plane_bounds, 0));
		cache_glyphs_vec.push_back(Assets::CreateGlyphEntry(cache_builder, gindex, &atlas_bounds, &plane_bounds, 0));

		++it;
	}
	// Save image.
	std::vector<uint8_t> image_data = makePng(bitmap);

	out_images_vec.push_back(Assets::CreateFontImageDirect(builder, image_width, &image_data));
	cache_images_vec.push_back(Assets::CreateFontImageDirect(cache_builder, image_width, &image_data));
	

	// Set resource file outputs.
	*out_glyphs = builder.CreateVector(out_glyphs_vec);
	*out_images = builder.CreateVector(out_images_vec);

	// Generate cache file.
	auto cache_glyphs_offset = cache_builder.CreateVector(cache_glyphs_vec);
	auto cache_images_offset = cache_builder.CreateVector(cache_images_vec);

	auto cache_font = Assets::CreateFont(cache_builder, cache_builder.CreateString(name), glyph_scale, *out_em_size, cache_glyphs_offset, cache_images_offset);
	cache_builder.Finish(cache_font, "FONT");

	// Write cache file to disk.
	fs::create_directories(cache_path.parent_path());
	std::ofstream cache_file(cache_path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!cache_file.is_open() || !cache_file.good())
		throw "Failed to write cache file!";

	cache_file.write((const char*)cache_builder.GetBufferPointer(), cache_builder.GetSize());
	cache_file.close();
}

std::vector<uint8_t> FontLoader::makePng(msdfgen::BitmapConstRef<BitmapStorageType, MSDFChannels> bitmap)
{
	std::vector<msdfgen::byte> pixels(bitmap.width * bitmap.height);
	for (int y = 0; y < bitmap.height; ++y)
		memcpy(&pixels[bitmap.width * y], bitmap(0, bitmap.height - y - 1), bitmap.width);

	std::vector<uint8_t> out;

	stbi_write_png_to_func([](void* context, void* data, int size) {
		std::vector<uint8_t>* out_ptr = reinterpret_cast<std::vector<uint8_t>*>(context);

		if (out_ptr->size() > 0)
			throw std::runtime_error("Write called more than once.");

		out_ptr->resize(size);
		memcpy_s(out_ptr->data(), out_ptr->size(), data, size);

	}, &out, bitmap.width, bitmap.height, MSDFChannels, pixels.data(), bitmap.width * MSDFChannels);

	if (out.size() == 0)
		throw std::runtime_error("Failed to write PNG");

	return out;
}
