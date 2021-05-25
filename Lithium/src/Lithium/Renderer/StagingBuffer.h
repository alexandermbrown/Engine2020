#pragma once

#include "Lithium/Core/Memory.h"

namespace Li
{
	class StagingBuffer
	{
	public:
		virtual ~StagingBuffer() = default;

		static Ref<StagingBuffer> Create(uint32_t size);
	};
}
