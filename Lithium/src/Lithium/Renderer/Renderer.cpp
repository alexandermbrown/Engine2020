#include "lipch.h"
#include "Renderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include "ShaderInterop/FrameCB.h"
#include "ShaderInterop/CameraCB.h"
#include "ShaderInterop/TransformCB.h"
#include "ShaderInterop/FontCB.h"
#include "ShaderInterop/ParticleEmitterSI.h"

#include "glm/gtc/matrix_transform.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Li
{
	Unique<Renderer::RendererData> Renderer::s_Data;

	void Renderer::Init()
	{
		s_Data = MakeUnique<Renderer::RendererData>();
		const GraphicsFactory* factory = GraphicsFactory::Get();

		Window& window = Application::Get().GetWindow();
		int window_width = window.GetWidth();
		int window_height = window.GetHeight();

		window.GetContext()->SetClearColor({ 0.7f, 0.7f, 0.7f, 1.0f });
		s_Data->FrameUB = factory->MakeUniformBuffer(LI_CB_GETBINDSLOT(FrameCB), sizeof(FrameCB));
		s_Data->FrameUB->BindBase();

		s_Data->CameraUB = factory->MakeUniformBuffer(LI_CB_GETBINDSLOT(CameraCB), sizeof(CameraCB));
		s_Data->CameraUB->BindBase();

		s_Data->TransformMatrixUB = factory->MakeUniformBuffer(LI_CB_GETBINDSLOT(TransformCB), sizeof(TransformCB));
		s_Data->TransformMatrixUB->BindBase();

		s_Data->FontUB = factory->MakeUniformBuffer(LI_CB_GETBINDSLOT(FontCB), sizeof(FontCB));
		s_Data->FontUB->BindBase();

		s_Data->Sorter = MakeRef<GPUSort>();

		s_Data->SceneCamera = nullptr;
		s_Data->UICamera = MakeUnique<Camera>();
		s_Data->UICamera->SetOrtho(0.0f, (float)window_width, 0.0f, (float)window_height);
		
		// IMMEDIATE QUAD RENDERING //
		s_Data->TextureShader = ResourceManager::GetShader("shader_splash");
		VertexBufferLayout quad_layout {
			{ ShaderDataType::Float2, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 }
		};
		Pipeline::Spec quad_spec;
		quad_spec.VertexBufferCount = 1;
		quad_spec.Layouts[0] = quad_layout;
		quad_spec.ShaderRef = s_Data->TextureShader;
		s_Data->QuadPipeline = factory->MakePipeline(quad_spec);

		constexpr float quad_vertices[16] = {
			0.0, 0.0, 0.0f, 0.0f,
			1.0, 0.0, 1.0f, 0.0f,
			1.0, 1.0, 1.0f, 1.0f,
			0.0, 1.0, 0.0f, 1.0f
		};
		s_Data->QuadVB = factory->MakeVertexBuffer(quad_vertices, sizeof(quad_vertices), BufferUsage::StaticDraw);
		s_Data->QuadVB->SetLayout(quad_layout);

		constexpr uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		s_Data->QuadIB = factory->MakeIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t), BufferUsage::StaticDraw);


		// 2D BATCH RENDERER //
		s_Data->SceneRenderer2D = MakeUnique<BatchRenderer2D>(glm::vec2{ 0.5f, 0.5f }, s_Data->CameraUB, s_Data->TransformMatrixUB);
		s_Data->UIRenderer2D = MakeUnique<BatchRenderer2D>(glm::vec2{ 0.0f, 0.0f }, s_Data->CameraUB, s_Data->TransformMatrixUB);
		s_Data->SceneLineRenderer = MakeUnique<LineBatchRenderer>(s_Data->CameraUB);

		Ref<TextureAtlas> texture_white_atlas = ResourceManager::GetTextureAtlas("texture_white_atlas");
		s_Data->SceneRenderer2D->AddTextureAtlas(texture_white_atlas);
		s_Data->UIRenderer2D->AddTextureAtlas(texture_white_atlas);

		// 3D MODELS //
		s_Data->ModelShader = ResourceManager::GetShader("shader_model");

		VertexBufferLayout model_layout {
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float3, "NORMAL", 1 }
		};
		Pipeline::Spec model_spec;
		model_spec.VertexBufferCount = 1;
		model_spec.Layouts[0] = model_layout;
		model_spec.ShaderRef = s_Data->ModelShader;
		s_Data->ModelPipeline = factory->MakePipeline(model_spec);

		// FONT //
		s_Data->FontShader = ResourceManager::GetShader("shader_label");
		VertexBufferLayout font_layout {
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 },
			{ ShaderDataType::Float, "TEXINDEX", 2 }
		};

		Pipeline::Spec font_spec;
		font_spec.VertexBufferCount = 1;
		font_spec.Layouts[0] = font_layout;
		font_spec.ShaderRef = s_Data->FontShader;
		s_Data->FontPipeline = factory->MakePipeline(font_spec);

		// POST PROCESSING //
		//s_Data->PostprocessFB = Framebuffer::Create(window_width, window_height);
		
	}

	void Renderer::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		s_Data->SceneRenderer2D->AddTextureAtlas(atlas);
		s_Data->UIRenderer2D->AddTextureAtlas(atlas);
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

	void Renderer::BeginScene(const Camera* camera)
	{
		s_Data->SceneCamera = camera;

		CameraCB camera_cb;
		camera_cb.u_View = camera->GetViewMatrix();
		camera_cb.u_Proj = camera->GetProjectionMatrix();
		camera_cb.u_ViewProj = camera->GetViewProjectionMatrix();
		camera_cb.u_CameraPos = camera->GetPosition();
		s_Data->CameraUB->SetData(&camera_cb);

		s_Data->SceneRenderer2D->BeginScene();
		s_Data->SceneLineRenderer->BeginScene();
		s_Data->SceneBegun = true;
	}

	void Renderer::EndScene()
	{
		s_Data->SceneBegun = false;
		s_Data->SceneRenderer2D->EndScene();
		s_Data->SceneLineRenderer->EndScene();
	}

	void Renderer::BeginUI()
	{
		CameraCB camera_cb;
		camera_cb.u_View = s_Data->UICamera->GetViewMatrix();
		camera_cb.u_Proj = s_Data->UICamera->GetProjectionMatrix();
		camera_cb.u_ViewProj = s_Data->UICamera->GetViewProjectionMatrix();
		camera_cb.u_CameraPos = s_Data->UICamera->GetPosition();
		s_Data->CameraUB->SetData(&camera_cb);

		s_Data->UIRenderer2D->BeginScene();
	}

	void Renderer::EndUI()
	{
		s_Data->UIRenderer2D->EndScene();
	}

	void Renderer::SubmitQuad(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop)
	{
		LI_CORE_ASSERT(s_Data->SceneBegun, "Scene not begun");
		s_Data->SceneRenderer2D->Submit(texture_alias, color, transform, crop);
	}

	void Renderer::SubmitModel(const Model* model, const glm::mat4& transform)
	{
		LI_CORE_ASSERT(s_Data->SceneBegun, "Scene not begun");

		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		s_Data->ModelShader->Bind();

		TransformCB transform_cb;
		transform_cb.u_Transform = transform;
		s_Data->TransformMatrixUB->SetData(&transform_cb);

		s_Data->CameraUB->Bind(ShaderType::Vertex);
		s_Data->CameraUB->Bind(ShaderType::Fragment);
		s_Data->TransformMatrixUB->Bind(ShaderType::Vertex);
		
		Pipeline::BindArray vertex_buffers;
		vertex_buffers[0] = model->m_Vertices;
		s_Data->ModelPipeline->Bind(vertex_buffers);
		model->m_Indices->Bind();

		context->SetDrawMode(DrawMode::Triangles);
		context->DrawIndexed(model->m_Indices->GetCount());
		context->UnbindVertexArray();
	}

	void Renderer::SubmitLabel(const Label* label, const glm::mat4& transform, const glm::vec4& color)
	{
		LI_CORE_ASSERT(s_Data->SceneBegun, "Scene not begun");
		RenderLabel(label, transform, color);
	}

	void Li::Renderer::SubmitLine(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2)
	{
		LI_CORE_ASSERT(s_Data->SceneBegun, "Scene not begun");
		s_Data->SceneLineRenderer->Submit(color, point1, point2);
	}

	void Li::Renderer::SubmitCircle(const glm::vec4& color, const glm::vec3& center, float radius)
	{
		LI_CORE_ASSERT(s_Data->SceneBegun, "Scene not begun");
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

	void Renderer::RenderQuadImmediate(const Texture* texture, const glm::mat4& transform)
	{
		RenderQuad(texture, transform, s_Data->SceneCamera->GetViewProjectionMatrix());
	}

	void Renderer::UISubmitQuad(const std::string& texture_alias, const glm::vec4& color, const glm::mat4& transform, bool crop)
	{
		s_Data->UIRenderer2D->Submit(texture_alias, color, transform, crop);
	}

	void Renderer::UISubmitLabel(const Label* label, const glm::mat4& transform, const glm::vec4& color)
	{
		RenderLabel(label, transform, color);
	}

	void Renderer::UIRenderQuadImmediate(const Texture* texture, const glm::mat4& transform)
	{
		RenderQuad(texture, transform, s_Data->UICamera->GetViewProjectionMatrix());
	}

	void Renderer::Resize(int width, int height)
	{
		//s_Data->PostprocessFB->Resize(width, height);
		s_Data->UICamera->SetOrtho(0, (float)width, 0, (float)height);
	}

	void Renderer::RenderQuad(const Texture* texture, const glm::mat4& transform, const glm::mat4& view_projection)
	{
		LI_CORE_ASSERT(!s_Data->SceneBegun, "Do not render quad during scene pass.");
		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		s_Data->TextureShader->Bind();

		TransformCB transform_cb;
		transform_cb.u_Transform = transform;
		s_Data->TransformMatrixUB->SetData(&transform_cb);

		CameraCB camera_cb;
		camera_cb.u_ViewProj = view_projection;
		s_Data->CameraUB->SetData(&camera_cb);
		s_Data->CameraUB->Bind(ShaderType::Vertex);
		s_Data->TransformMatrixUB->Bind(ShaderType::Vertex);

		texture->Bind();

		Pipeline::BindArray vertex_buffers;
		vertex_buffers[0] = s_Data->QuadVB;
		s_Data->QuadPipeline->Bind(vertex_buffers);
		s_Data->QuadIB->Bind();

		context->SetDrawMode(DrawMode::Triangles);
		context->DrawIndexed(s_Data->QuadIB->GetCount());
		context->UnbindVertexArray();
	}

	void Renderer::RenderLabel(const Label* label, const glm::mat4& transform, const glm::vec4& color)
	{
		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		s_Data->FontShader->Bind();

		TransformCB transform_cb;
		transform_cb.u_Transform = transform;
		s_Data->TransformMatrixUB->SetData(&transform_cb);

		FontCB font_cb;
		font_cb.u_Color = color;
		font_cb.u_DistanceFactor = label->GetDistanceFactor();
		s_Data->FontUB->SetData(&font_cb);

		s_Data->CameraUB->Bind(ShaderType::Vertex);
		s_Data->TransformMatrixUB->Bind(ShaderType::Vertex);
		s_Data->FontUB->Bind(ShaderType::Fragment);

		const auto& font = label->GetFont();
		const auto& textures = font->GetTextures();
		for (int i = 0; i < font->GetTextureCount(); i++)
			textures[i]->Bind(i);
		
		Pipeline::BindArray vertex_buffers;
		vertex_buffers[0] = label->m_VertexBuffer;

		s_Data->FontPipeline->Bind(vertex_buffers);
		label->m_IndexBuffer->Bind();

		context->SetDrawMode(DrawMode::Triangles);
		context->DrawIndexed(label->m_IndexBuffer->GetCount());
		context->UnbindVertexArray();
	}
}
