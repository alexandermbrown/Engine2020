#include "lipch.h"
#include "OpenGLIndirectBuffer.h"

#include "OpenGLCore.h"
#include "ConvertOpenGL.h"
#include "glad/glad.h"

namespace Li
{
	OpenGLIndirectBuffer::OpenGLIndirectBuffer(uint32_t size, IndirectTarget target)
		: m_Size(size)
	{
		switch (target)
		{
		case IndirectTarget::Compute:
			m_Target = GL_DISPATCH_INDIRECT_BUFFER;
			break;
		case IndirectTarget::Draw:
			m_Target = GL_DRAW_INDIRECT_BUFFER;
			break;
		default:
			m_Target = 0;
			LI_CORE_ASSERT(false, "Bad indirect target.");
		}

		glBindBuffer(m_Target, m_BufferID);
		glBufferData(m_Target, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void OpenGLIndirectBuffer::Bind(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
		glBindBuffer(m_Target, m_BufferID);
	}

	void OpenGLIndirectBuffer::DispatchComputeIndirect(uint32_t args_offset) const
	{
		glDispatchComputeIndirect(args_offset);
	}

	void OpenGLIndirectBuffer::DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const
	{
		uint64_t offset = args_offset;
		static_assert(sizeof(offset) == sizeof(const void*));
		glDrawArraysIndirect(ConvertOpenGL::DrawMode(mode), reinterpret_cast<const void*>(offset));
	}
}
