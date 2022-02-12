#pragma once

#include "Lithium/Renderer/IndirectBuffer.h"
#include "GLBuffer.h"

namespace Li
{
	class GLIndirectBuffer : public IndirectBuffer, public GLBuffer
	{
	public:
		GLIndirectBuffer(uint32_t size, IndirectTarget target);
		virtual ~GLIndirectBuffer() = default;

		virtual void Bind(uint32_t slot) const override;
		virtual void DispatchComputeIndirect(uint32_t args_offset) const override;
		virtual void DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const override;

	private:
		uint32_t m_Target;
		uint32_t m_Size;
	};
}
