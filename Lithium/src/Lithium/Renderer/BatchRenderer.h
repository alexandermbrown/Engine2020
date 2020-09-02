#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/TextureAtlas.h"
#include "Lithium/Renderer/OrthographicCamera.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include "glm/glm.hpp"

namespace li
{
	struct BatchData
	{
		glm::mat4 Transform;
		glm::vec4 AtlasBounds;
		glm::vec4 Color;
		float TextureIndex;
	};

	class BatchRenderer
	{
	public:
		BatchRenderer(glm::vec2 quadOrigin = glm::vec2(1.0f));
		virtual ~BatchRenderer() = default;

		void AddTextureAtlas(Ref<TextureAtlas> atlas);
		void SetUniformBuffer(Ref<UniformBuffer> viewProjBuffer);

		void BeginScene();
		void EndScene();
		void Submit(
			const std::string& texture_alias,
			const glm::vec4& color,
			const glm::mat4& transform,
			bool crop);

	private:
		void Flush();

		glm::vec2 m_QuadOrigin;
		
		Ref<Shader> m_Shader;

		static constexpr int MaxBatchInstances = 16384;
		static constexpr int MaxBatchTextures = 8;

		uint32_t m_InstanceCount;
		int m_BatchAtlasCount;

		std::vector<Ref<TextureAtlas>> m_Atlases;
		std::unordered_map<std::string, int> m_AtlasIndices;

		std::array<Ref<TextureAtlas>, MaxBatchTextures> m_BatchAtlases;
		std::array<BatchData, MaxBatchInstances> m_InstanceData;
		Ref<VertexBuffer> m_InstanceBuffer;
		Ref<VertexArray> m_InstanceVA;
	};
}