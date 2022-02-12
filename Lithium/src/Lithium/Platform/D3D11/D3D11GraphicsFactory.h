#pragma once
#include "Lithium/Renderer/GraphicsFactory.h"

namespace Li
{
	class D3D11GraphicsFactory : public GraphicsFactory
	{
	public:
		virtual GraphicsAPI GetAPI() const override { return GraphicsAPI::D3D11; };

		virtual Ref<Pipeline> MakePipeline(const Pipeline::Spec& spec) const override;

		virtual Ref<VertexBuffer> MakeVertexBufferEmpty(uint32_t size, BufferUsage usage) const override;
		virtual Ref<VertexBuffer> MakeVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage) const override;
		virtual Ref<IndexBuffer> MakeIndexBufferEmpty(uint32_t size, BufferUsage usage) const override;
		virtual Ref<IndexBuffer> MakeIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage) const override;

		virtual Ref<Framebuffer> MakeFramebuffer(int width, int height) const override;
		virtual Ref<UniformBuffer> MakeUniformBuffer(uint32_t slot, uint32_t size) const override;
		virtual Ref<ReadbackBuffer> MakeReadbackBuffer(uint32_t size) const override;
		virtual Ref<ShaderBuffer> MakeShaderBuffer(const void* data, uint32_t size, uint32_t stride, ShaderBufferType type) const override;
		virtual Ref<IndirectBuffer> MakeIndirectBuffer(uint32_t size, IndirectTarget target) const override;

		virtual Ref<Texture2D> MakeTexture2D(int width, int height, int channels, void* data, const TextureProps& props, bool dynamic = false) const override;
		virtual Ref<Texture2D> MakeTexture2DFromFile(const std::string& path, int desired_channels, const TextureProps& props) const override;
		virtual Ref<Texture2D> MakeTexture2DFromEncoded(size_t image_size, const uint8_t* encoded_data, int desired_channels, const TextureProps& props) const override;

		virtual Unique<Window> MakeWindow(const WindowProps& props) const override;
		virtual Unique<ImGuiRenderer> MakeImGuiRenderer() const override;
	};
}
