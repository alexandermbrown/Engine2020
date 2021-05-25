#include "lipch.h"
#include "FontLoader.h"

namespace Li::Loaders
{
	Ref<Font> LoadFont(const Assets::Font* font)
	{
		FontProps props;
		props.GlyphWidth = font->glyph_width();
		props.DistanceGradient = font->distance_gradient();

		const auto* saved_glyphs = font->glyphs();
		Font::GlyphPropsMap glyph_props(saved_glyphs->size());
		for (const Assets::GlyphEntry* glyph : *saved_glyphs)
		{
			const Assets::Vec2* texture_offset = glyph->texture_offset();
			uint32_t codepoint = glyph->codepoint();
			auto& glyph_prop = glyph_props[codepoint];
			glyph_prop.texture_offset = { texture_offset->x(), texture_offset->y() };
			glyph_prop.texture_index = glyph->texture_index();
		}

		Font::TextureArray textures;
		int i = 0;
		for (const Assets::FontImage* image : *font->images())
		{
			const flatbuffers::Vector<uint8_t>* image_vec = image->image();
			textures[i++] = Texture2D::Create(image_vec->size(), image_vec->data(), 4, WrapType::ClampToEdge, WrapType::ClampToEdge, FilterType::Linear, FilterType::Linear);
		}
		
		const auto* ttf = font->ttf();
		return MakeRef<Font>(font->name()->str(), props, std::move(glyph_props), std::move(textures), i, (const char*)ttf->data(), (uint32_t)ttf->size());
	}
}
