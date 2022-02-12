#include "lipch.h"
#include "GLShaderBuffer.h"

#include "GLCore.h"
#include "glad/glad.h"

namespace Li
{
	GLShaderBuffer::GLShaderBuffer(const void* data, uint32_t size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);

		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);

		static int count = 1;
		Log::CoreDebug("Create SSBO {}", count++);
	}

	void GLShaderBuffer::BindBase(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
	}
}
