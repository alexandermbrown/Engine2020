#pragma once

#include "Lithium/Renderer/IndirectBuffer.h"
#include "OpenGLBuffer.h"

namespace Li
{
	class OpenGLIndirectBuffer : public IndirectBuffer, public OpenGLBuffer
	{
	public:
		OpenGLIndirectBuffer(uint32_t size, IndirectTarget target);
		virtual ~OpenGLIndirectBuffer() = default;

		virtual void Bind(uint32_t slot) const override;
		virtual void DispatchComputeIndirect(uint32_t args_offset) const override;
		virtual void DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const override;

	private:
		uint32_t m_Target;
		uint32_t m_Size;
	};
}
