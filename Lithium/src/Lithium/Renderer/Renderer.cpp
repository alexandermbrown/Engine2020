#include "lipch.h"
#include "Renderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include "ShaderInterop/FrameCB.h"
#include "ShaderInterop/ViewProjCB.h"
#include "ShaderInterop/TransformCB.h"
#include "ShaderInterop/FontCB.h"
#include "ShaderInterop/ParticleEmitterSI.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Li
{
	Unique<Renderer::RendererData> Renderer::s_Data;

	void Renderer::Init()
	{
		s_Data = MakeUnique<Renderer::RendererData>();

		Window& window = Application::Get().GetWindow();
		window.GetContext()->SetDepthTest(false);
		window.GetContext()->SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		s_Data->FrameUB = UniformBuffer::Create(LI_CB_GETBINDSLOT(FrameCB), sizeof(FrameCB));
		s_Data->FrameUB->BindBase();

		s_Data->ViewProjUB = UniformBuffer::Create(LI_CB_GETBINDSLOT(ViewProjCB), sizeof(ViewProjCB));
		s_Data->ViewProjUB->BindBase();

		s_Data->TransformMatrixUB = UniformBuffer::Create(LI_CB_GETBINDSLOT(TransformCB), sizeof(TransformCB));
		s_Data->TransformMatrixUB->BindBase();

		s_Data->FontUB = UniformBuffer::Create(LI_CB_GETBINDSLOT(FontCB), sizeof(FontCB));
		s_Data->FontUB->BindBase();

		s_Data->Camera = nullptr;

		s_Data->TextureShader = ResourceManager::GetShader("shader_splash");

		s_Data->UICamera = MakeUnique<Camera>();
		s_Data->UICamera->SetOrtho(0.0f, (float)window.GetWidth(), 0.0f, (float)window.GetHeight());
		
		//////////////////////////////////
		// Create Textured Quad Buffers //
		//////////////////////////////////

		s_Data->QuadVA = VertexArray::Create();

		float quadVertices[16] = {
			0.0, 0.0, 0.0f, 0.0f,
			1.0, 0.0, 1.0f, 0.0f,
			1.0, 1.0, 1.0f, 1.0f,
			0.0, 1.0, 0.0f, 1.0f
		};

		Ref<VertexBuffer> quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices), BufferUsage::StaticDraw);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		quadVB->SetLayout({
			{ ShaderDataType::Float2, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 }
		});

		s_Data->QuadVA->SetIndexBuffer(quadIB);
		s_Data->QuadVA->AddVertexBuffer(quadVB);
		s_Data->QuadVA->Finalize(s_Data->TextureShader);
	}

	void Renderer::InitPostResourceLoad()
	{
		s_Data->Sorter = MakeRef<GPUSort>();

		s_Data->FontShader = ResourceManager::GetShader("shader_label");

		s_Data->SceneRenderer = MakeUnique<BatchRenderer>(glm::vec2{ 0.5f, 0.5f }, s_Data->ViewProjUB, s_Data->TransformMatrixUB);
		s_Data->UIRenderer = MakeUnique<BatchRenderer>(glm::vec2{ 0.0f, 0.0f }, s_Data->ViewProjUB, s_Data->TransformMatrixUB);
		s_Data->SceneLineRenderer = MakeUnique<LineBatchRenderer>(s_Data->ViewProjUB);

		Ref<TextureAtlas> texture_white_atlas = ResourceManager::GetTextureAtlas("texture_white_atlas");
		s_Data->SceneRenderer->AddTextureAtlas(texture_white_atlas);
		s_Data->UIRenderer->AddTextureAtlas(texture_white_atlas);
		s_Data->ResourcesLoaded = true;
	}

	void Renderer::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		s_Data->SceneRenderer->AddTextureAtlas(atlas);
		s_Data->UIRenderer->AddTextureAtlas(atlas);
	}

	void Renderer::BeginFrame(Duration::us run_time, Duration::us delta_time)
	{
		s_Data->FrameNumber++;
		Li::GraphicsContext* context = Li::Application::Get().GetWindow().GetContext();
		context->BindDefaultRenderTarget();
		context->Clear();
		
		FrameCB frame_cb;
		frame_cb.u_RunTime = Duration::Cast<Duration::fsec>(run_time).count();
		frame_cb.u_DeltaTime = Duration::Cast<Duration::fsec>(delta_time).count();
		s_Data->FrameUB->SetData(&frame_cb);
	}

	void Renderer::BeginScene(Camera* camera)
	{
		s_Data->Camera = camera;

		ViewProjCB view_proj;
		view_proj.u_ViewProj = camera->GetViewProjectionMatrix();
		s_Data->ViewProjUB->SetData(&view_proj);

		s_Data->SceneRenderer->BeginScene();
		s_Data->SceneLineRenderer->BeginScene();
	}

	void Renderer::EndScene()
	{
		s_Data->SceneRenderer->EndScene();
		s_Data->SceneLineRenderer->EndScene();
	}

	void Renderer::BeginUI()
	{
		ViewProjCB view_proj;
		view_proj.u_ViewProj = s_Data->UICamera->GetViewProjectionMatrix();
		s_Data->ViewProjUB->SetData(&view_proj);

		s_Data->UIRenderer->BeginScene();
	}

	void Renderer::EndUI()
	{
		s_Data->UIRenderer->EndScene();
	}

	void Renderer::SubmitTextured(const std::string& textureAlias, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer->Submit(textureAlias, glm::vec4(1.0f), transform, crop);
	}

	void Renderer::SubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer->Submit("texture_white", color, transform, false);
	}

	void Renderer::SubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->SceneRenderer->Submit(textureAlias, color, transform, crop);
	}

	void Renderer::SubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		RenderLabel(label, transform, color);
	}

	void Li::Renderer::SubmitLine(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2)
	{
		s_Data->SceneLineRenderer->Submit(color, point1, point2);
	}

	void Li::Renderer::SubmitCircle(const glm::vec4& color, const glm::vec3& center, float radius)
	{
		glm::vec3 prev_point = {
				center.x + radius,
				center.y,
				center.z
		};

		constexpr int resolution = 32;
		for (int i = 1; i <= resolution; i++)
		{
			float angle = (float)i / (float)resolution * 2.0f * (float)M_PI;

			glm::vec3 current_point = {
				center.x + std::cos(angle) * radius,
				center.y + std::sin(angle) * radius,
				center.z
			};
			Li::Renderer::SubmitLine(color, prev_point, current_point);
			prev_point = current_point;
		}
	}

	void Renderer::Submit(const Ref<Texture>& texture, const glm::mat4& transform)
	{
		RenderQuad(texture, transform, s_Data->Camera->GetViewProjectionMatrix());
	}

	void Renderer::UISubmitTextured(const std::string& textureAlias, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer->Submit(textureAlias, glm::vec4(1.0f), transform, crop);
	}

	void Renderer::UISubmitColored(const glm::vec4& color, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer->Submit("texture_white", color, transform, false);
	}

	void Renderer::UISubmitColoredTexture(const std::string& textureAlias, const glm::vec4& color, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		s_Data->UIRenderer->Submit(textureAlias, color, transform, crop);
	}

	void Renderer::UISubmitLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		LI_CORE_ASSERT(s_Data->ResourcesLoaded, "Resources not loaded!");
		RenderLabel(label, transform, color);
	}

	void Renderer::UISubmit(const Ref<Texture>& texture, const glm::mat4& transform)
	{
		RenderQuad(texture, transform, s_Data->UICamera->GetViewProjectionMatrix());
	}

	void Renderer::Resize(int width, int height)
	{
		s_Data->UICamera->SetOrtho(0, (float)width, 0, (float)height);
	}

	void Renderer::RenderQuad(const Ref<Texture>& texture, const glm::mat4& transform, const glm::mat4& view_projection)
	{
		s_Data->TextureShader->Bind();

		TransformCB transform_cb;
		transform_cb.u_Transform = transform;
		s_Data->TransformMatrixUB->SetData(&transform_cb);

		ViewProjCB view_proj;
		view_proj.u_ViewProj = view_projection;
		s_Data->ViewProjUB->SetData(&view_proj);

		s_Data->ViewProjUB->Bind(ShaderType::Vertex);
		s_Data->TransformMatrixUB->Bind(ShaderType::Vertex);
		texture->Bind();
		s_Data->QuadVA->Bind();
		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		context->SetDrawMode(DrawMode::Triangles);
		context->DrawIndexed(s_Data->QuadVA->GetIndexBuffer()->GetCount());
		context->UnbindVertexArray();
	}

	void Renderer::RenderLabel(const Ref<Label>& label, const glm::mat4& transform, const glm::vec4& color)
	{
		const Ref<VertexArray>& vertex_array = label->GetVertexArray();
		s_Data->FontShader->Bind();

		TransformCB transform_cb;
		transform_cb.u_Transform = transform;
		s_Data->TransformMatrixUB->SetData(&transform_cb);

		FontCB font_cb;
		font_cb.u_Color = color;
		font_cb.u_DistanceFactor = label->GetDistanceFactor();
		s_Data->FontUB->SetData(&font_cb);

		s_Data->ViewProjUB->Bind(ShaderType::Vertex);
		s_Data->TransformMatrixUB->Bind(ShaderType::Vertex);
		s_Data->FontUB->Bind(ShaderType::Fragment);

		const auto& font = label->GetFont();
		const auto& textures = font->GetTextures();
		for (int i = 0; i < font->GetTextureCount(); i++)
			textures[i]->Bind(i);
		
		vertex_array->Bind();
		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		context->SetDrawMode(DrawMode::Triangles);
		context->DrawIndexed(vertex_array->GetIndexBuffer()->GetCount());
		context->UnbindVertexArray();
	}
}
