#include "lipch.h"
#include "Label.h"

#include "Lithium/Renderer/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Li
{
	Label::Label(const char* utf8_text, int point_size, Ref<Font> font, bool dynamic, int excess)
		: m_PointSize(point_size), m_Font(font), m_BufferLength(0u), m_Dynamic(dynamic), m_Excess(excess)
	{
		m_DistanceFactor = font->GetProps().DistanceGradient * (float)m_PointSize + 2.0f;
		m_Buffer = hb_buffer_create();

		LoadLayout(utf8_text);
		CreateRenderingBuffers(dynamic);
	}

	Label::~Label()
	{
		hb_buffer_destroy(m_Buffer);
	}

	void Label::SetText(const char* utf8_text)
	{
		if (!m_Dynamic)
		{
			Log::CoreError("Label must be dynamic to set text!");
			return;
		}

		hb_buffer_clear_contents(m_Buffer);
		m_GlyphVertices.clear();
		m_GlyphIndices.clear();

		bool resize = LoadLayout(utf8_text);
		if (resize)
		{
			Log::CoreWarn("Increasing text buffer size.");
			CreateRenderingBuffers(true);
		}
		else
		{
			UpdateRenderingBuffers();
		}
	}

	bool Label::LoadLayout(const char* utf8_text)
	{
		bool resize = false;

		hb_buffer_add_utf8(m_Buffer, utf8_text, -1, 0, -1);
		hb_buffer_guess_segment_properties(m_Buffer);

		hb_shape(m_Font->GetHBFont(m_PointSize), m_Buffer, NULL, 0);

		unsigned int buffer_length = hb_buffer_get_length(m_Buffer);

		if (buffer_length > m_BufferLength)
		{
			// Reserve extra length if specified.
			m_BufferLength = buffer_length + m_Excess;
			m_GlyphVertices.reserve((size_t)m_BufferLength * 4);
			m_GlyphIndices.reserve((size_t)m_BufferLength * 6);
			resize = true;
		}

		hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(m_Buffer, NULL);
		hb_glyph_position_t* glyph_position = hb_buffer_get_glyph_positions(m_Buffer, NULL);

		const FontProps& font_info = m_Font->GetProps();
		hb_position_t current_x = 0;
		hb_position_t current_y = 0;

		for (unsigned int i = 0; i < buffer_length; i++)
		{
			float x_position = (current_x + glyph_position[i].x_offset) / 64.0f;
			float y_position = (current_y + glyph_position[i].y_offset) / 64.0f;

			float left = x_position - (4.0f * (float)m_PointSize / (float)font_info.GlyphWidth);
			float right = left + (float)m_PointSize * 1.33f;
			float bottom = y_position - (4.0f * (float)m_PointSize / (float)font_info.GlyphWidth);
			float top = bottom + (float)m_PointSize * 1.33f;

			const GlyphProps& glyph_props = m_Font->GetGlyphProps(glyph_info[i].codepoint);
			float texture_width = (float)m_Font->GetTextures()[glyph_props.texture_index]->GetWidth();

			float texture_left = glyph_props.texture_offset.x + 0.002f;
			float texture_bottom = glyph_props.texture_offset.y + 0.002f;
			float texture_right = glyph_props.texture_offset.x + font_info.GlyphWidth / texture_width - 0.002f;
			float texture_top = glyph_props.texture_offset.y + font_info.GlyphWidth / texture_width - 0.002f;

			uint32_t index_offset = i * 4;
			m_GlyphIndices.insert(m_GlyphIndices.end(), {
				index_offset, index_offset + 1, index_offset + 2,
				index_offset, index_offset + 2, index_offset + 3,
			});

			GlyphVertex vertex;
			vertex.TexIndex = static_cast<float>(glyph_props.texture_index);

			vertex.Position = { left, bottom, (float)i / (float)buffer_length * 0.001f };
			vertex.TexCoords = { texture_left, texture_bottom };
			m_GlyphVertices.push_back(vertex);

			vertex.Position = { right, bottom, (float)i / (float)buffer_length * 0.001f };
			vertex.TexCoords = { texture_right, texture_bottom };
			m_GlyphVertices.push_back(vertex);

			vertex.Position = { right, top, (float)i / (float)buffer_length * 0.001f };
			vertex.TexCoords = { texture_right, texture_top };
			m_GlyphVertices.push_back(vertex);

			vertex.Position = { left, top, (float)i / (float)buffer_length * 0.001f };
			vertex.TexCoords = { texture_left, texture_top };
			m_GlyphVertices.push_back(vertex);

			current_x += glyph_position[i].x_advance;
			current_y += glyph_position[i].y_advance;
		}
		return resize;
	}

	void Label::CreateRenderingBuffers(bool dynamic)
	{
		m_VertexArray = VertexArray::Create();

		if (dynamic)
		{
			m_VertexBuffer = VertexBuffer::Create((uint32_t)(sizeof(GlyphVertex) * m_BufferLength * 4), BufferUsage::DynamicDraw);
			m_VertexBuffer->SetSubData((float*)m_GlyphVertices.data(), (uint32_t)(sizeof(GlyphVertex) * m_GlyphVertices.size()), 0, true);

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((uint32_t)(sizeof(uint32_t) * m_BufferLength * 6), BufferUsage::DynamicDraw);
			indexBuffer->SetSubData(m_GlyphIndices.data(), (uint32_t)(sizeof(uint32_t) * m_GlyphIndices.size()), 0, true);

			m_VertexArray->SetIndexBuffer(indexBuffer);
		}
		else
		{
			m_VertexBuffer = VertexBuffer::Create((float*)m_GlyphVertices.data(),
				(uint32_t)(sizeof(GlyphVertex) * m_GlyphVertices.size()),
				BufferUsage::StaticDraw);

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_GlyphIndices.data(),
				(uint32_t)m_GlyphIndices.size(),
				BufferUsage::StaticDraw);

			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 },
			{ ShaderDataType::Float, "TEXINDEX", 2 }
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->Finalize(Renderer::GetFontShader());
		m_VertexArray->Unbind();
	}

	void Label::UpdateRenderingBuffers()
	{
		m_VertexBuffer->SetSubData((float*)m_GlyphVertices.data(),
			(uint32_t)(sizeof(GlyphVertex) * m_GlyphVertices.size()),
			0, true);

		m_VertexArray->GetIndexBuffer()->SetSubData(m_GlyphIndices.data(),
			(uint32_t)(sizeof(uint32_t) * m_GlyphIndices.size()), 0, true);
	}
}
