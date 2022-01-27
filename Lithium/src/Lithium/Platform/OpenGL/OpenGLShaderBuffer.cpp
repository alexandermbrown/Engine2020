#include "lipch.h"
#include "OpenGLShaderBuffer.h"

#include "OpenGLCore.h"
#include "glad/glad.h"

namespace Li
{
	OpenGLShaderBuffer::OpenGLShaderBuffer(const void* data, uint32_t size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);

		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);

		static int count = 1;
		Log::CoreDebug("Create SSBO {}", count++);
	}

	void OpenGLShaderBuffer::BindBase(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
	}
}
