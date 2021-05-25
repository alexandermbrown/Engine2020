#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Texture.h"

#include "glm/glm.hpp"
#include "hb.h"

#include <unordered_map>

namespace Li
{
	struct FontProps
	{
		uint16_t GlyphWidth;
		float DistanceGradient;
	};

	struct GlyphProps
	{
		glm::vec2 texture_offset;
		int texture_index;
	};

	class Font
	{
	public:
		static constexpr int MaxTextures = 8;
		using TextureArray = std::array<Ref<Texture2D>, MaxTextures>;
		using GlyphPropsMap = std::unordered_map<uint32_t, GlyphProps>;

		Font(const std::string& name, const FontProps& props, GlyphPropsMap&& glyph_props, TextureArray&& textures, int texture_count, const char* ttf_data, uint32_t ttf_size);
		virtual ~Font();

		inline const FontProps& GetProps() const { return m_Props; }
		const GlyphProps& GetGlyphProps(int16_t character) const;
		inline const std::array<Ref<Texture2D>, MaxTextures>& GetTextures() const { return m_Textures; }
		inline int GetTextureCount() const { return m_TextureCount; }

		hb_font_t* GetHBFont(int point_size);

	private:
		std::string m_Name;
		FontProps m_Props;
		GlyphPropsMap m_GlyphProps;
		TextureArray m_Textures;
		int m_TextureCount;

		std::unordered_map<int, hb_font_t*> m_Fonts;
		hb_face_t* m_Face;
	};
}
