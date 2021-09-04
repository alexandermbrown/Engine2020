#pragma once

#include "Lithium/Renderer/Buffer.h"

namespace Li
{
	class OpenGLBuffer
	{
	public:
		OpenGLBuffer();
		virtual ~OpenGLBuffer();

		inline uint32_t GetID() const { return m_BufferID; }

	protected:
		uint32_t m_BufferID;
	};

	class OpenGLVertexBuffer : public VertexBuffer, public OpenGLBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, BufferUsage usage);
		OpenGLVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage);
		virtual ~OpenGLVertexBuffer() = default;

		virtual void Bind() const override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

		virtual void SetSubData(const float* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		VertexBufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer, public OpenGLBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t size, BufferUsage usage);
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage);
		virtual ~OpenGLIndexBuffer() = default;

		virtual void Bind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

		virtual void SetSubData(const uint32_t* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		uint32_t m_Count;
	};
}
