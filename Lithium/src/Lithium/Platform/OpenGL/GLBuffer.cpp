#include "lipch.h"
#include "Lithium/Platform/OpenGL/GLBuffer.h"
#include "Lithium/Platform/OpenGL/ConvertGL.h"

#include "GLCore.h"

#include "glad/glad.h"

namespace Li
{
	GLBuffer::GLBuffer()
	{
		glGenBuffers(1, &m_BufferID);
	}

	GLBuffer::~GLBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_BufferID));
	}

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	GLVertexBuffer::GLVertexBuffer(uint32_t size, BufferUsage usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertGL::BufferUsage(usage));
	}

	GLVertexBuffer::GLVertexBuffer(const float* vertices, uint32_t size, BufferUsage usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		glBufferData(GL_ARRAY_BUFFER, size, vertices, ConvertGL::BufferUsage(usage));
	}

	void GLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void GLVertexBuffer::SetSubData(const float* data, uint32_t size, uint32_t offset, bool discard)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
		if (discard) access |= GL_MAP_INVALIDATE_BUFFER_BIT;

		void* dest = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, access);
		LI_CORE_ASSERT(dest, "Error mapping buffer.");
		memcpy(dest, data, size);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	GLIndexBuffer::GLIndexBuffer(uint32_t size, BufferUsage usage)
		: m_Count(0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertGL::BufferUsage(usage));
	}

	GLIndexBuffer::GLIndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage)
		: m_Count(count)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, ConvertGL::BufferUsage(usage));
	}

	void GLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void GLIndexBuffer::SetSubData(const uint32_t* data, uint32_t size, uint32_t offset, bool discard)
	{
		m_Count = size / sizeof(uint32_t);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);

		GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
		if (discard) access |= GL_MAP_INVALIDATE_BUFFER_BIT;

		void* dest = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, size, access);
		LI_CORE_ASSERT(dest, "Error mapping buffer.");
		memcpy(dest, data, size);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}
