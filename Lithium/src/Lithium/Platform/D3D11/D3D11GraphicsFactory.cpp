#include "lipch.h"
#include "D3D11GraphicsFactory.h"

#include "D3D11Pipeline.h"
#include "D3D11Buffer.h"
#include "D3D11Framebuffer.h"
#include "D3D11UniformBuffer.h"
#include "D3D11ReadbackBuffer.h"
#include "D3D11ShaderBuffer.h"
#include "D3D11IndirectBuffer.h"
#include "D3D11Texture.h"
#include "D3D11Window.h"
#include "D3D11ImGuiRenderer.h"

namespace Li
{
	Ref<Pipeline> D3D11GraphicsFactory::MakePipeline(const Pipeline::Spec& spec) const
	{
		return MakeRef<D3D11Pipeline>(spec);
	}

	Ref<VertexBuffer> D3D11GraphicsFactory::MakeVertexBufferEmpty(uint32_t size, BufferUsage usage) const
	{
		return MakeRef<D3D11VertexBuffer>(size, usage);
	}
	Ref<VertexBuffer> D3D11GraphicsFactory::MakeVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage) const
	{
		return MakeRef<D3D11VertexBuffer>(vertices, size, usage);
	}

	Ref<IndexBuffer> D3D11GraphicsFactory::MakeIndexBufferEmpty(uint32_t size, BufferUsage usage) const
	{
		return MakeRef<D3D11IndexBuffer>(size, usage);
	}
	Ref<IndexBuffer> D3D11GraphicsFactory::MakeIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage) const
	{
		return MakeRef<D3D11IndexBuffer>(indices, count, usage);
	}

	Ref<Framebuffer> D3D11GraphicsFactory::MakeFramebuffer(int width, int height) const
	{
		return MakeRef<D3D11Framebuffer>(width, height);
	}
	Ref<UniformBuffer> D3D11GraphicsFactory::MakeUniformBuffer(uint32_t slot, uint32_t size) const
	{
		return MakeRef<D3D11UniformBuffer>(slot, size);
	}
	Ref<ReadbackBuffer> D3D11GraphicsFactory::MakeReadbackBuffer(uint32_t size) const
	{
		return MakeRef<D3D11ReadbackBuffer>(size);
	}
	Ref<ShaderBuffer> D3D11GraphicsFactory::MakeShaderBuffer(const void* data, uint32_t size, uint32_t stride, ShaderBufferType type) const
	{
		return MakeRef<D3D11ShaderBuffer>(data, size, stride, type);
	}
	Ref<IndirectBuffer> D3D11GraphicsFactory::MakeIndirectBuffer(uint32_t size, IndirectTarget target) const
	{
		return MakeRef<D3D11IndirectBuffer>(size, target);
	}

	Ref<Texture2D> D3D11GraphicsFactory::MakeTexture2D(int width, int height, int channels, void* data, const TextureProps& props, bool dynamic) const
	{
		return MakeRef<D3D11Texture2D>(width, height, channels, data, props, dynamic, false);
	}
	Ref<Texture2D> D3D11GraphicsFactory::MakeTexture2DFromFile(const std::string& path, int desired_channels, const TextureProps& props) const
	{
		return MakeRef<D3D11Texture2D>(path, desired_channels, props);
	}
	Ref<Texture2D> D3D11GraphicsFactory::MakeTexture2DFromEncoded(size_t image_size, const uint8_t* encoded_data, int desired_channels, const TextureProps& props) const
	{
		return MakeRef<D3D11Texture2D>(image_size, encoded_data, desired_channels, props);
	}

	Unique<Window> D3D11GraphicsFactory::MakeWindow(const WindowProps& props) const
	{
		return MakeUnique<D3D11Window>(props);
	}
	Unique<ImGuiRenderer> D3D11GraphicsFactory::MakeImGuiRenderer() const
	{
		return MakeUnique<D3D11ImGuiRenderer>();
	}
}
