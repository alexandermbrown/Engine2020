#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Camera.h"
#include "Lithium/Renderer/Pipeline.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include "glm/glm.hpp"

namespace Li
{
	class LineBatchRenderer
	{
	public:
		LineBatchRenderer(const Ref<UniformBuffer>& camera_buffer);
		virtual ~LineBatchRenderer() = default;

		void BeginScene();
		void EndScene();
		void Submit(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2);

	private:
		void Flush();
		void AddVertex(const glm::vec4& color, const glm::vec3& point);

		struct LineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		static constexpr int MaxBatchVertices = 16384;

		Ref<Shader> m_Shader;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<Pipeline> m_Pipeline;
		Ref<UniformBuffer> m_CameraUB;

		uint32_t m_VertexCount;
		int m_BatchAtlasCount;

		std::array<LineVertex, MaxBatchVertices> m_VertexData;
	};
}
