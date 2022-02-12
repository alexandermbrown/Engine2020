#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/RendererEnums.h"
#include <stdint.h>

namespace Li
{
	enum class ShaderBufferType
	{
		Structured,
		Raw
	};

	class ShaderBuffer
	{
	public:
		virtual ~ShaderBuffer() = default;

		virtual void BindBase(uint32_t slot) const = 0;
		virtual void BindSRV(ShaderType type, uint32_t slot) const = 0;
		virtual void BindUAV(uint32_t slot) const = 0;
	};
}
