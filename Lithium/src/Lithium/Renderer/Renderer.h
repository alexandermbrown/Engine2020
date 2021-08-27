#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Camera.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Shader.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Renderer/BatchRenderer.h"
#include "Lithium/Renderer/LineBatchRenderer.h"
#include "Lithium/Renderer/RenderStage.h"
#include "Lithium/Renderer/Text/Label.h"
#include "Lithium/Utility/Time.h"

#include "glm/glm.hpp"

namespace Li
{
	class Renderer
	{
	public:
		static void Init();
		static void InitPostResourceLoad();

		static void Shutdown();

		static void AddTextureAtlas(Ref<TextureAtlas> atlas);

		static void BeginFrame(Duration::us run_time, Duration::us delta_time);

		static void BeginScene(Camera* camera);
		static void EndScene();

		static void BeginUI();
		static void EndUI();

		static void SubmitTextured(const std::string& texture_alias, const glm::mat4& transform, bool crop = false);

		static void SubmitColored(const glm::vec4& color, const glm::mat4& transform);

		static void SubmitColoredTexture(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop = false);

		static void SubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		static void SubmitLine(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2);

		static void SubmitCircle(const glm::vec4& color, const glm::vec3& center, float radius);

		static void Submit(const Ref<Texture>& texture, const glm::mat4& transform);

		static void UISubmitTextured(const std::string& texture_alias, const glm::mat4& transform, bool crop = false);

		static void UISubmitColored(const glm::vec4& color, const glm::mat4& transform);

		static void UISubmitColoredTexture(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop = false);

		static void UISubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		static void UISubmit(const Ref<Texture>& texture, const glm::mat4& transform);

		static void Resize(int width, int height);

		inline static const Ref<UniformBuffer>& GetFrameUniformBuffer() { return s_Data->FrameUB; }
		inline static const Ref<UniformBuffer>& GetViewProjBuffer() { return s_Data->ViewProjUB; }
		inline static const Ref<UniformBuffer>& GetTransformBuffer() { return s_Data->TransformMatrixUB; }
		inline static const Ref<Shader>& GetFontShader() { return s_Data->FontShader; }

	private:
		static void RenderQuad(const Ref<Texture>& texture, const glm::mat4& transform, const glm::mat4& view_projection);
		static void RenderLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color);

		struct RendererData
		{
			Unique<BatchRenderer> SceneRenderer = nullptr;
			Unique<BatchRenderer> UIRenderer = nullptr;
			Unique<LineBatchRenderer> SceneLineRenderer = nullptr;

			Ref<Shader> FontShader;

			Ref<Shader> TextureShader;
			Ref<VertexArray> QuadVA;

			Ref<UniformBuffer> FrameUB;
			Ref<UniformBuffer> ViewProjUB;
			Ref<UniformBuffer> TransformMatrixUB;
			Ref<UniformBuffer> FontUB;

			Camera* Camera;
			Unique<Li::Camera> UICamera;

			bool ResourcesLoaded{ false };
			uint64_t FrameNumber{ 0 };
		};

		static Unique<RendererData> s_Data;
	};
}
