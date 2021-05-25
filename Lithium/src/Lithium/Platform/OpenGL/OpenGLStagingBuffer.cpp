#include "lipch.h"
#include "OpenGLStagingBuffer.h"

#include "OpenGLCore.h"
#include "glad/glad.h"

namespace Li
{
	OpenGLStagingBuffer::OpenGLStagingBuffer(uint32_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_COPY_WRITE_BUFFER, m_RendererID);

		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLStagingBuffer::~OpenGLStagingBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
}
