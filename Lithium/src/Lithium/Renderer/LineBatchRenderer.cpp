#include "lipch.h"
#include "LineBatchRenderer.h"

#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace Li
{
	LineBatchRenderer::LineBatchRenderer(const Ref<UniformBuffer>& camera_buffer)
		: m_VertexCount(0), m_CameraUB(camera_buffer)
	{
		// Max must be an even number because each line has two points.
		static_assert(MaxBatchVertices % 2 == 0);
		static_assert(sizeof(LineVertex) == sizeof(glm::vec3) + sizeof(glm::vec4));

		m_Shader = ResourceManager::GetShader("shader_line");

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float4, "COLOR", 1 }
		};

		m_VertexBuffer = GraphicsFactory::Get()->MakeVertexBufferEmpty(sizeof(LineVertex) * MaxBatchVertices, BufferUsage::DynamicDraw);
		m_VertexBuffer->SetLayout(layout);

		Pipeline::Spec spec;
		spec.VertexBufferCount = 1;
		spec.Layouts[0] = layout;
		spec.ShaderRef = m_Shader;
		m_Pipeline = GraphicsFactory::Get()->MakePipeline(spec);
	}

	void LineBatchRenderer::BeginScene()
	{
		m_VertexCount = 0;
	}

	void LineBatchRenderer::EndScene()
	{
		Flush();
	}

	void LineBatchRenderer::Submit(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2)
	{
		if (m_VertexCount >= MaxBatchVertices)
			Flush();
		AddVertex(color, point1);
		AddVertex(color, point2);
	}

	void LineBatchRenderer::Flush()
	{
		LI_CORE_ASSERT(m_VertexCount % 2 == 0, "Must have an even number of vertices.");

		if (m_VertexCount > 0)
		{
			m_Shader->Bind();
			m_VertexBuffer->SetSubData((float*)m_VertexData.data(), sizeof(LineVertex) * m_VertexCount, 0, true);
			m_CameraUB->Bind(ShaderType::Vertex);

			Pipeline::BindArray vertex_buffers;
			vertex_buffers[0] = m_VertexBuffer;
			m_Pipeline->Bind(vertex_buffers);

			GraphicsContext* context = Application::Get().GetWindow().GetContext();
			context->SetDrawMode(Li::DrawMode::Lines);
			context->DrawArrays(m_VertexCount);
			context->UnbindVertexArray();

			m_VertexCount = 0;
		}
	}

	void LineBatchRenderer::AddVertex(const glm::vec4& color, const glm::vec3& point)
	{
		m_VertexData[m_VertexCount] = { point, color };
		m_VertexCount++;
	}
}
