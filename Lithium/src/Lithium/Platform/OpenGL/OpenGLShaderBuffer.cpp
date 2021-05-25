#include "lipch.h"
#include "OpenGLShaderBuffer.h"

#include "OpenGLCore.h"
#include "glad/glad.h"

namespace Li
{
	OpenGLShaderBuffer::OpenGLShaderBuffer(const void* data, uint32_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);

		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	OpenGLShaderBuffer::~OpenGLShaderBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLShaderBuffer::BindBase(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID);
	}
}
