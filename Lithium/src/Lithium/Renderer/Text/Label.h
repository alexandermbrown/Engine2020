#pragma once

#include "Font.h"
#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/Pipeline.h"

#include "glm/glm.hpp"
#include "harfbuzz/hb.h"

#include <vector>

namespace Li
{
	struct GlyphVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		float TexIndex;
	};

	class Label
	{
	public:
		Label(const char* utf8_text, int point_size, Ref<Font> font, bool dynamic, int excess = 0);
		virtual ~Label();

		inline int GetPointSize() const { return m_PointSize; }
		inline float GetDistanceFactor() const { return m_DistanceFactor; }
		inline const Ref<Font>& GetFont() const { return m_Font; }

		void SetText(const char* utf8_text);

	private:
		bool LoadLayout(const char* utf8_text);
		void CreateRenderingBuffers(bool dynamic);
		void UpdateRenderingBuffers();

		int m_PointSize;
		float m_DistanceFactor;
		Ref<Font> m_Font;

		bool m_Dynamic;
		int m_Excess;

		unsigned int m_BufferLength;
		std::vector<GlyphVertex> m_GlyphVertices;
		std::vector<uint32_t> m_GlyphIndices;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		hb_buffer_t* m_Buffer;

		friend class Renderer;
	};
}
