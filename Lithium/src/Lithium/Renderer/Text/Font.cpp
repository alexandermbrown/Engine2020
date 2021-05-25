#include "lipch.h"
#include "Font.h"

#include "hb-ot.h"

namespace Li
{
	Font::Font(const std::string& name, const FontProps& props, GlyphPropsMap&& glyph_props, TextureArray&& textures, int texture_count, const char* ttf_data, uint32_t ttf_size)
		: m_Name(name), m_Props(props), m_GlyphProps(glyph_props), m_Textures(textures), m_TextureCount(texture_count)
	{
		hb_blob_t* blob = hb_blob_create(ttf_data, ttf_size, HB_MEMORY_MODE_DUPLICATE, nullptr, 0);
		m_Face = hb_face_create(blob, 0);
		hb_blob_destroy(blob);
	}

	Font::~Font()
	{
		for (auto [pt, font] : m_Fonts)
		{
			hb_font_destroy(font);
		}
		
		hb_face_destroy(m_Face);
	}

	const GlyphProps& Font::GetGlyphProps(int16_t character) const
	{
		auto iter = m_GlyphProps.find(character);
		if (iter != m_GlyphProps.end())
			return iter->second;
		else
			return m_GlyphProps.at('?');
	}

	hb_font_t* Font::GetHBFont(int point_size)
	{
		auto iter = m_Fonts.find(point_size);
		if (iter != m_Fonts.end())
		{
			return iter->second;
		}
		else
		{
			hb_font_t* font = hb_font_create(m_Face);
			hb_ot_font_set_funcs(font);
			hb_font_set_scale(font, point_size * 64, point_size * 64);
			m_Fonts[point_size] = font;
			return font;
		}
	}
}
