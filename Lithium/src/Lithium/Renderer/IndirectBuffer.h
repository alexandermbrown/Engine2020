#pragma once

#include "RendererEnums.h"
#include "Lithium/Core/Memory.h"
#include <stdint.h>

namespace Li
{
	enum class IndirectTarget
	{
		Compute,
		Draw
	};

	class IndirectBuffer
	{
	public:
		virtual ~IndirectBuffer() = default;

		virtual void Bind(uint32_t slot) const = 0;
		virtual void DispatchComputeIndirect(uint32_t args_offset) const = 0;
		virtual void DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const = 0;

		static Ref<IndirectBuffer> Create(uint32_t size, uint32_t stride, IndirectTarget target);
	};

	struct IndirectDrawArgsInstanced
	{
		uint32_t VertexCountPerInstance = 0;
		uint32_t InstanceCount = 0;
		uint32_t StartVertexLocation = 0;
		uint32_t StartInstanceLocation = 0;
	};

	struct IndirectDrawArgsIndexedInstanced
	{
		uint32_t IndexCountPerInstance = 0;
		uint32_t InstanceCount = 0;
		uint32_t StartIndexLocation = 0;
		int32_t BaseVertexLocation = 0;
		uint32_t StartInstanceLocation = 0;
	};

	struct IndirectDispatchArgs
	{
		uint32_t ThreadGroupCountX = 0;
		uint32_t ThreadGroupCountY = 0;
		uint32_t ThreadGroupCountZ = 0;
	};
}

