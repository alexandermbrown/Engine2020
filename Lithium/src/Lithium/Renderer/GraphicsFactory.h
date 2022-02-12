#pragma once
#include "Pipeline.h"
#include "Buffer.h"
#include "Framebuffer.h"
#include "UniformBuffer.h"
#include "ReadbackBuffer.h"
#include "ShaderBuffer.h"
#include "IndirectBuffer.h"
#include "Texture.h"
#include "ImGuiRenderer.h"

#include "Lithium/Core/Memory.h"
#include "Lithium/Core/Window.h"

namespace Li
{
	enum class GraphicsAPI
	{
		None = 0,
		OpenGL,
		D3D11
	};

	class GraphicsFactory
	{
	public:
		GraphicsFactory();

		static inline const GraphicsFactory* Get() { return s_Instance;  }
		static Unique<GraphicsFactory> Create(GraphicsAPI api);

		virtual ~GraphicsFactory() { s_Instance = nullptr; };
		virtual GraphicsAPI GetAPI() const = 0;

		virtual Ref<Pipeline> MakePipeline(const Pipeline::Spec& spec) const = 0;

		virtual Ref<VertexBuffer> MakeVertexBufferEmpty(uint32_t size, BufferUsage usage) const = 0;
		virtual Ref<VertexBuffer> MakeVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage) const = 0;
		virtual Ref<IndexBuffer> MakeIndexBufferEmpty(uint32_t size, BufferUsage usage) const = 0;
		virtual Ref<IndexBuffer> MakeIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage) const = 0;

		virtual Ref<Framebuffer> MakeFramebuffer(int width, int height) const = 0;
		virtual Ref<UniformBuffer> MakeUniformBuffer(uint32_t slot, uint32_t size) const = 0;
		virtual Ref<ReadbackBuffer> MakeReadbackBuffer(uint32_t size) const = 0;
		virtual Ref<ShaderBuffer> MakeShaderBuffer(const void* data, uint32_t size, uint32_t stride, ShaderBufferType type) const = 0;
		virtual Ref<IndirectBuffer> MakeIndirectBuffer(uint32_t size, IndirectTarget target) const = 0;

		virtual Ref<Texture2D> MakeTexture2D(int width, int height, int channels, void* data, const TextureProps& props, bool dynamic = false) const = 0;
		virtual Ref<Texture2D> MakeTexture2DFromFile(const std::string& path, int desired_channels, const TextureProps& props) const = 0;
		virtual Ref<Texture2D> MakeTexture2DFromEncoded(size_t image_size, const uint8_t* encoded_data, int desired_channels, const TextureProps& props) const = 0;

		virtual Unique<Window> MakeWindow(const WindowProps& props) const = 0;
		virtual Unique<ImGuiRenderer> MakeImGuiRenderer() const = 0;

		static GraphicsFactory* s_Instance;
	};
}
