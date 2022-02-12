#include "lipch.h"
#include "GLReadbackBuffer.h"
#include "GLShaderBuffer.h"

#include "GLCore.h"
#include "glad/glad.h"

namespace Li
{
	GLReadbackBuffer::GLReadbackBuffer(uint32_t size)
	{
		glBindBuffer(GL_COPY_WRITE_BUFFER, m_BufferID);
		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_DYNAMIC_READ);
	}

	void GLReadbackBuffer::Readback(Ref<ShaderBuffer> buffer, uint32_t size)
	{
		GLShaderBuffer* gl_buffer = static_cast<GLShaderBuffer*>(buffer.get());
		uint32_t buffer_id = gl_buffer->GetID();
		
		glBindBuffer(GL_COPY_READ_BUFFER, buffer_id);
		glBindBuffer(GL_COPY_WRITE_BUFFER, m_BufferID);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
	}

	void GLReadbackBuffer::GetData(void* dest, uint32_t size)
	{
		glBindBuffer(GL_COPY_WRITE_BUFFER, m_BufferID);
		const void* src = glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, (GLsizeiptr)size, GL_MAP_READ_BIT);
		LI_CORE_ASSERT(src, "Failed to map buffer.");
		memcpy(dest, src, size);
		glUnmapBuffer(GL_COPY_WRITE_BUFFER);
	}
}
