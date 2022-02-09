#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Texture.h"

#include "glm/glm.hpp"
#include "harfbuzz/hb.h"

#include <unordered_map>

namespace Li
{
	struct FontProps
	{
		float GlyphScale;
		float EmSize;
	};

	struct GlyphProps
	{
		glm::vec4 atlas_bounds;
		glm::vec4 plane_bounds;
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
		const GlyphProps& GetGlyphProps(uint32_t gindex) const;
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
