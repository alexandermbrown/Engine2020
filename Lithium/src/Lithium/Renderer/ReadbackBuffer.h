#pragma once

#include "Lithium/Core/Memory.h"

#include "ShaderBuffer.h"

namespace Li
{
	class ReadbackBuffer
	{
	public:
		virtual ~ReadbackBuffer() = default;

		virtual void Readback(Ref<ShaderBuffer> buffer, uint32_t size) = 0;
		virtual void GetData(void* dest, uint32_t size) = 0;

		static Ref<ReadbackBuffer> Create(uint32_t size);
	};
}
