#include "lipch.h"
#include "BatchRenderer2D.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include "glm/gtc/type_ptr.hpp"

namespace Li
{
	BatchRenderer2D::BatchRenderer2D(glm::vec2 quadOrigin, const Ref<UniformBuffer>& camera_buffer, const Ref<UniformBuffer>& transform_buffer)
		: m_QuadCount(0), m_CameraUB(camera_buffer), m_TransformUB(transform_buffer), m_Vertices(nullptr)
	{
		static_assert(sizeof(BatchVertex) == sizeof(BatchVertex::Position) + sizeof(BatchVertex::TexCoord) + sizeof(BatchVertex::Color) + sizeof(BatchVertex::TexIndex));
		static_assert(sizeof(BatchVertex[MaxBatchQuads * 4]) == sizeof(BatchVertex) * 4 * MaxBatchQuads);

		m_Shader = ResourceManager::GetShader("shader_batched");

		m_VertexPositions[0] = { -quadOrigin.x       , -quadOrigin.y       , 0.0f, 1.0f };
		m_VertexPositions[1] = { -quadOrigin.x + 1.0f, -quadOrigin.y       , 0.0f, 1.0f };
		m_VertexPositions[2] = { -quadOrigin.x + 1.0f, -quadOrigin.y + 1.0f, 0.0f, 1.0f };
		m_VertexPositions[3] = { -quadOrigin.x       , -quadOrigin.y + 1.0f, 0.0f, 1.0f };

		std::vector<uint32_t> indices;
		indices.resize(MaxBatchQuads * 6);

		int offset = 0;
		for (int index = 0; index < MaxBatchQuads * 6; index += 6)
		{
			indices[index]     = offset + 0;
			indices[index + 1] = offset + 1;
			indices[index + 2] = offset + 2;
			indices[index + 3] = offset + 0;
			indices[index + 4] = offset + 2;
			indices[index + 5] = offset + 3;
			offset += 4;
		}
		m_IndexBuffer = GraphicsFactory::Get()->MakeIndexBuffer(indices.data(), MaxBatchQuads * 6, BufferUsage::StaticDraw);

		m_Vertices = new BatchVertex[MaxBatchQuads * 4];
		m_VertexBuffer = GraphicsFactory::Get()->MakeVertexBufferEmpty(sizeof(BatchVertex[MaxBatchQuads * 4]), BufferUsage::DynamicDraw);

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float, "TEXINDEX", 1 },
			{ ShaderDataType::Float2, "TEXCOORD", 2 },
			{ ShaderDataType::Float4, "COLOR", 3 }
		};

		m_VertexBuffer->SetLayout(layout);

		Pipeline::Spec spec;
		spec.VertexBufferCount = 1;
		spec.Layouts[0] = layout;
		spec.ShaderRef = m_Shader;
		m_Pipeline = GraphicsFactory::Get()->MakePipeline(spec);
	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		if (m_Vertices)
		{
			delete[] m_Vertices;
			m_Vertices = nullptr;
		}
	}

	void BatchRenderer2D::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		for (auto&& [texture, bounds] : atlas->GetEntries())
		{
			m_Atlases[texture] = atlas;
		}
	}

	void BatchRenderer2D::BeginScene()
	{
		m_QuadCount = 0;
		m_BatchAtlasCount = 0;
	}

	void BatchRenderer2D::Submit(
		const std::string& texture_name,
		const glm::vec4& color,
		const glm::mat4& transform,
		bool crop)
	{
		if (m_QuadCount >= MaxBatchQuads)
			Flush();

		auto it = m_Atlases.find(texture_name);
		if (it == m_Atlases.end())
		{
			Log::CoreError("Texture {} not found.", texture_name);
			return;
		}

		float tex_index;
		bool found = false;
		TextureAtlas* target_atlas = it->second.get();
		for (int i = m_BatchAtlasCount - 1; i >= 0; --i)
		{
			TextureAtlas* batch_atlas = m_BatchAtlases[i];
			if (batch_atlas == target_atlas)
			{
				found = true;
				tex_index = (float)i;
				break;
			}
		}

		if (!found)
		{
			if (m_BatchAtlasCount >= m_BatchAtlases.size())
				Flush();

			m_BatchAtlases[m_BatchAtlasCount] = target_atlas;
			tex_index = static_cast<float>(m_BatchAtlasCount);
			m_BatchAtlasCount++;
		}
		
		glm::vec4 bounds = target_atlas->GetBounds(texture_name);
		if (crop)
		{
			float quad_width = transform[0][0];
			float quad_height = transform[1][1];

			float quad_ratio = quad_width / quad_height;
			float texture_ratio = bounds.z / bounds.w;

			glm::vec2 texture_scale;
			if (quad_ratio > texture_ratio)
				texture_scale = { 1.0f, texture_ratio / quad_ratio };
			else
				texture_scale = { quad_ratio / texture_ratio, 1.0f };

			bounds = {
				bounds.x + bounds.z / 2.0f * (1.0f - texture_scale.x),
				bounds.y + bounds.w / 2.0f * (1.0f - texture_scale.y),
				bounds.z * texture_scale.x,
				bounds.w * texture_scale.y
			};
		}

		// Loop through each quad vertex.
		for (int i = 0; i < 4; i++)
		{
			BatchVertex& vertex = m_Vertices[m_QuadCount * 4 + i];

			vertex.Position = transform * m_VertexPositions[i];
			vertex.TexCoord.x = bounds.x + bounds.z * m_TexCoords[i].x;
			vertex.TexCoord.y = bounds.y + bounds.w * m_TexCoords[i].y;
			vertex.Color = color;
			vertex.TexIndex = tex_index;
		}

		m_QuadCount++;
	}

	void BatchRenderer2D::Flush()
	{
		if (m_QuadCount > 0)
		{
			m_Shader->Bind();

			// Bind all batch atlas textures.
			for (int slot = 0; slot < m_BatchAtlasCount; slot++)
			{
				m_BatchAtlases[slot]->Bind(slot);
			}

			m_VertexBuffer->SetSubData(
				reinterpret_cast<float*>(m_Vertices),
				sizeof(BatchVertex) * m_QuadCount * 4,
				0, true
			);

			m_CameraUB->Bind(ShaderType::Vertex);
			m_TransformUB->Bind(ShaderType::Vertex);

			Pipeline::BindArray vertex_buffers;
			vertex_buffers[0] = m_VertexBuffer;
			m_Pipeline->Bind(vertex_buffers);
			m_IndexBuffer->Bind();

			GraphicsContext* context = Application::Get().GetWindow().GetContext();
			context->SetDrawMode(Li::DrawMode::Triangles);
			context->DrawIndexed(m_QuadCount * 6);
			context->UnbindVertexArray();
		}
		m_QuadCount = 0;
		m_BatchAtlasCount = 0;
	}
}
