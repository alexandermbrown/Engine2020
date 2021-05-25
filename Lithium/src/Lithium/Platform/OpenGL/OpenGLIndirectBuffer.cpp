#include "lipch.h"
#include "OpenGLIndirectBuffer.h"

#include "OpenGLCore.h"
#include "ConvertOpenGL.h"
#include "glad/glad.h"

namespace Li
{
	OpenGLIndirectBuffer::OpenGLIndirectBuffer(uint32_t size, uint32_t stride, IndirectTarget target)
		: m_Size(size), m_Stride(stride)
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

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(m_Target, m_RendererID);

		glBufferData(m_Target, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLIndirectBuffer::~OpenGLIndirectBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLIndirectBuffer::Bind(uint32_t slot) const
	{
		glBindBuffer(m_Target, m_RendererID);
	}

	void OpenGLIndirectBuffer::DispatchComputeIndirect(uint32_t args_offset) const
	{
		glDispatchComputeIndirect(args_offset);
	}

	void OpenGLIndirectBuffer::DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const
	{
		uint64_t offset = args_offset;
		static_assert(sizeof(offset) == sizeof(const void*));
		glMultiDrawArraysIndirect(ConvertOpenGL::DrawMode(mode), reinterpret_cast<const void*>(offset), m_Size / m_Stride, m_Stride);
	}
}
