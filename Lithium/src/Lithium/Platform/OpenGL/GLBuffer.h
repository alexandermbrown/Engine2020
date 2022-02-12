#pragma once

#include "Lithium/Renderer/Buffer.h"

namespace Li
{
	class GLBuffer
	{
	public:
		GLBuffer();
		virtual ~GLBuffer();

		inline uint32_t GetID() const { return m_BufferID; }

	protected:
		uint32_t m_BufferID;
	};

	class GLVertexBuffer : public VertexBuffer, public GLBuffer
	{
	public:
		GLVertexBuffer(uint32_t size, BufferUsage usage);
		GLVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage);
		virtual ~GLVertexBuffer() = default;

		virtual void Bind() const override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

		virtual void SetSubData(const float* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		VertexBufferLayout m_Layout;
	};

	class GLIndexBuffer : public IndexBuffer, public GLBuffer
	{
	public:
		GLIndexBuffer(uint32_t size, BufferUsage usage);
		GLIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage);
		virtual ~GLIndexBuffer() = default;

		virtual void Bind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

		virtual void SetSubData(const uint32_t* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		uint32_t m_Count;
	};
}
