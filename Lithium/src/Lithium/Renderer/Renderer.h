#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/BatchRenderer2D.h"
#include "Lithium/Renderer/Camera.h"
#include "Lithium/Renderer/GPUSort.h"
#include "Lithium/Renderer/LineBatchRenderer.h"
#include "Lithium/Renderer/Model.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Renderer/RenderStage.h"
#include "Lithium/Renderer/Shader.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/Text/Label.h"
#include "Lithium/Renderer/Pipeline.h"
#include "Lithium/Utility/Time.h"

#include "glm/glm.hpp"

namespace Li
{
	class Renderer
	{
	public:
		static void Init();

		static void Shutdown();

		static void AddTextureAtlas(Ref<TextureAtlas> atlas);

		static void BeginFrame(Duration::us run_time, Duration::us delta_time);

		static void BeginScene(Camera* camera);
		static void EndScene();

		static void BeginUI();
		static void EndUI();

		static void SubmitQuad(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop = false);
		static void SubmitModel(const Ref<Model>& model, const glm::mat4& transform);
		static void SubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);
		static void SubmitLine(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2);
		static void SubmitCircle(const glm::vec4& color, const glm::vec3& center, float radius);
		static void RenderQuadImmediate(const Ref<Texture>& texture, const glm::mat4& transform);

		static void UISubmitQuad(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop = false);
		static void UISubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);
		static void UIRenderQuadImmediate(const Ref<Texture>& texture, const glm::mat4& transform);

		static void Resize(int width, int height);

		inline static const Ref<UniformBuffer>& GetFrameUniformBuffer() { return s_Data->FrameUB; }
		inline static const Ref<UniformBuffer>& GetViewProjBuffer() { return s_Data->CameraUB; }
		inline static const Ref<UniformBuffer>& GetTransformBuffer() { return s_Data->TransformMatrixUB; }
		inline static const Ref<Shader>& GetFontShader() { return s_Data->FontShader; }
		inline static const Ref<GPUSort>& GetSorter() { return s_Data->Sorter; }

		inline static const Ref<Pipeline>& GetModelPipeline() { return s_Data->ModelPipeline; }
		inline static const Ref<Pipeline>& GetFontPipeline() { return s_Data->FontPipeline; }

	private:
		static void RenderQuad(const Ref<Texture>& texture, const glm::mat4& transform, const glm::mat4& view_projection);
		static void RenderLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		struct RendererData
		{
			Unique<BatchRenderer2D> SceneRenderer2D = nullptr;
			Unique<BatchRenderer2D> UIRenderer2D = nullptr;
			Unique<LineBatchRenderer> SceneLineRenderer = nullptr;

			Ref<Shader> TextureShader;
			Ref<Shader> ModelShader;
			Ref<Shader> FontShader;

			Ref<Pipeline> QuadPipeline;
			Ref<Pipeline> ModelPipeline;
			Ref<Pipeline> FontPipeline;

			Ref<VertexBuffer> QuadVB;
			Ref<IndexBuffer> QuadIB;

			Ref<UniformBuffer> FrameUB;
			Ref<UniformBuffer> CameraUB;
			Ref<UniformBuffer> TransformMatrixUB;
			Ref<UniformBuffer> FontUB;

			Ref<GPUSort> Sorter;

			Camera* Camera;
			Unique<Li::Camera> UICamera;

			uint64_t FrameNumber{ 0 };
			bool SceneBegun{ false };
		};

		static Unique<RendererData> s_Data;
	};
}
