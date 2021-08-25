#include "lipch.h"
#include "Lithium/Platform/OpenGL/OpenGLBuffer.h"
#include "Lithium/Platform/OpenGL/ConvertOpenGL.h"

#include "OpenGLCore.h"

#include "glad/glad.h"

namespace Li
{
	OpenGLBuffer::OpenGLBuffer()
	{
		glGenBuffers(1, &m_BufferID);
	}

	OpenGLBuffer::~OpenGLBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_BufferID));
	}

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, BufferUsage usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertOpenGL::BufferUsage(usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		glBufferData(GL_ARRAY_BUFFER, size, vertices, ConvertOpenGL::BufferUsage(usage));
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void OpenGLVertexBuffer::SetSubData(float* data, uint32_t size, uint32_t offset, bool discard)
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

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size, BufferUsage usage)
		: m_Count(0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertOpenGL::BufferUsage(usage));
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count, BufferUsage usage)
		: m_Count(count)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, ConvertOpenGL::BufferUsage(usage));
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void OpenGLIndexBuffer::SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard)
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
