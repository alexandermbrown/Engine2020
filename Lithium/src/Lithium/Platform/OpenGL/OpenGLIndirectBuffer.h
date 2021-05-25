#pragma once

#include "Lithium/Renderer/IndirectBuffer.h"

namespace Li
{
	class OpenGLIndirectBuffer : public IndirectBuffer
	{
	public:
		OpenGLIndirectBuffer(uint32_t size, uint32_t stride, IndirectTarget target);
		virtual ~OpenGLIndirectBuffer();

		virtual void Bind(uint32_t slot) const override;
		virtual void DispatchComputeIndirect(uint32_t args_offset) const override;
		virtual void DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Target;
		uint32_t m_Size;
		uint32_t m_Stride;
	};
}
