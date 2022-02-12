#include "lipch.h"
#include "GLUniformBuffer.h"

#include "GLCore.h"
#include "glad/glad.h"

namespace Li
{
	GLUniformBuffer::GLUniformBuffer(uint32_t slot, uint32_t size)
		: m_Slot(slot), m_Size(size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void GLUniformBuffer::BindBase() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, m_BufferID);
	}

	void GLUniformBuffer::SetData(const void* data)
	{
		constexpr GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
		void* dest = glMapBufferRange(GL_UNIFORM_BUFFER, 0, (GLsizeiptr)m_Size, access);
		LI_CORE_ASSERT(dest, "Failed to map buffer.");
		memcpy(dest, data, m_Size);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
}
