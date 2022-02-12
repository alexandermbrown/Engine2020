#include "lipch.h"
#include "GLIndirectBuffer.h"

#include "GLCore.h"
#include "ConvertGL.h"
#include "glad/glad.h"

namespace Li
{
	GLIndirectBuffer::GLIndirectBuffer(uint32_t size, IndirectTarget target)
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

	void GLIndirectBuffer::Bind(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
		glBindBuffer(m_Target, m_BufferID);
	}

	void GLIndirectBuffer::DispatchComputeIndirect(uint32_t args_offset) const
	{
		glDispatchComputeIndirect(args_offset);
	}

	void GLIndirectBuffer::DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const
	{
		uint64_t offset = args_offset;
		static_assert(sizeof(offset) == sizeof(const void*));
		glDrawArraysIndirect(ConvertGL::DrawMode(mode), reinterpret_cast<const void*>(offset));
	}
}
