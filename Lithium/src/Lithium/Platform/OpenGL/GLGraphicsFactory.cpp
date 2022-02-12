#include "lipch.h"
#include "GLGraphicsFactory.h"

#include "GLPipeline.h"
#include "GLBuffer.h"
#include "GLFramebuffer.h"
#include "GLUniformBuffer.h"
#include "GLReadbackBuffer.h"
#include "GLShaderBuffer.h"
#include "GLIndirectBuffer.h"
#include "GLTexture.h"
#include "GLWindow.h"
#include "GLImGuiRenderer.h"

namespace Li
{
	Ref<Pipeline> GLGraphicsFactory::MakePipeline(const Pipeline::Spec& spec) const
	{
		return MakeRef<GLPipeline>(spec);
	}

	Ref<VertexBuffer> GLGraphicsFactory::MakeVertexBufferEmpty(uint32_t size, BufferUsage usage) const
	{
		return MakeRef<GLVertexBuffer>(size, usage);
	}
	Ref<VertexBuffer> GLGraphicsFactory::MakeVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage) const
	{
		return MakeRef<GLVertexBuffer>(vertices, size, usage);
	}

	Ref<IndexBuffer> GLGraphicsFactory::MakeIndexBufferEmpty(uint32_t size, BufferUsage usage) const
	{
		return MakeRef<GLIndexBuffer>(size, usage);
	}
	Ref<IndexBuffer> GLGraphicsFactory::MakeIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage) const
	{
		return MakeRef<GLIndexBuffer>(indices, count, usage);
	}

	Ref<Framebuffer> GLGraphicsFactory::MakeFramebuffer(int width, int height) const
	{
		return MakeRef<GLFramebuffer>(width, height);
	}
	Ref<UniformBuffer> GLGraphicsFactory::MakeUniformBuffer(uint32_t slot, uint32_t size) const
	{
		return MakeRef<GLUniformBuffer>(slot, size);
	}
	Ref<ReadbackBuffer> GLGraphicsFactory::MakeReadbackBuffer(uint32_t size) const
	{
		return MakeRef<GLReadbackBuffer>(size);
	}
	Ref<ShaderBuffer> GLGraphicsFactory::MakeShaderBuffer(const void* data, uint32_t size, uint32_t stride, ShaderBufferType type) const
	{
		return MakeRef<GLShaderBuffer>(data, size);
	}
	Ref<IndirectBuffer> GLGraphicsFactory::MakeIndirectBuffer(uint32_t size, IndirectTarget target) const
	{
		return MakeRef<GLIndirectBuffer>(size, target);
	}

	Ref<Texture2D> GLGraphicsFactory::MakeTexture2D(int width, int height, int channels, void* data, const TextureProps& props, bool dynamic) const
	{
		return MakeRef<GLTexture2D>(width, height, channels, data, props);
	}
	Ref<Texture2D> GLGraphicsFactory::MakeTexture2DFromFile(const std::string& path, int desired_channels, const TextureProps& props) const
	{
		return MakeRef<GLTexture2D>(path, desired_channels, props);
	}
	Ref<Texture2D> GLGraphicsFactory::MakeTexture2DFromEncoded(size_t image_size, const uint8_t* encoded_data, int desired_channels, const TextureProps& props) const
	{
		return MakeRef<GLTexture2D>(image_size, encoded_data, desired_channels, props);
	}

	Unique<Window> GLGraphicsFactory::MakeWindow(const WindowProps& props) const
	{
		return MakeUnique<GLWindow>(props);
	}
	Unique<ImGuiRenderer> GLGraphicsFactory::MakeImGuiRenderer() const
	{
		return MakeUnique<GLImGuiRenderer>();
	}
}
