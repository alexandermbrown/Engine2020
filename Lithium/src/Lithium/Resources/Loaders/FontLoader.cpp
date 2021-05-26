#include "lipch.h"
#include "FontLoader.h"

namespace Li::Loaders
{
	Ref<Font> LoadFont(const Assets::Font* font)
	{
		FontProps props;
		props.GlyphScale = (float)font->glyph_scale();
		props.EmSize = font->em_size();
		props.DistanceGradient = 0.9f / props.EmSize;

		const auto* saved_glyphs = font->glyphs();
		Font::GlyphPropsMap glyph_props(saved_glyphs->size());
		for (const Assets::GlyphEntry* glyph : *saved_glyphs)
		{
			const Assets::Vec4* atlas_bounds = glyph->atlas_bounds();
			const Assets::Vec4* plane_bounds = glyph->plane_bounds();
			uint32_t gindex = glyph->gindex();
			auto& glyph_prop = glyph_props[gindex];
			glyph_prop.atlas_bounds = { atlas_bounds->x(), atlas_bounds->y(), atlas_bounds->z(), atlas_bounds->w() };
			glyph_prop.plane_bounds = { plane_bounds->x(), plane_bounds->y(), plane_bounds->z(), plane_bounds->w() };
			glyph_prop.texture_index = glyph->texture_index();
		}

		Font::TextureArray textures;
		int i = 0;
		for (const Assets::FontImage* image : *font->images())
		{
			const flatbuffers::Vector<uint8_t>* image_vec = image->image();
			textures[i++] = Texture2D::Create(image_vec->size(), image_vec->data(), 4,
				WrapType::ClampToEdge, WrapType::ClampToEdge, FilterType::Linear, FilterType::Linear);
		}
		
		const auto* ttf = font->ttf();
		return MakeRef<Font>(font->name()->str(), props, std::move(glyph_props), std::move(textures), i, (const char*)ttf->data(), (uint32_t)ttf->size());
	}
}
