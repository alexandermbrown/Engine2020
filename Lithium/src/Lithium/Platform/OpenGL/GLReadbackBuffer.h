#pragma once

#include "GLBuffer.h"
#include "Lithium/Renderer/ReadbackBuffer.h"

namespace Li
{
	class GLReadbackBuffer : public ReadbackBuffer, public GLBuffer
	{
	public:
		GLReadbackBuffer(uint32_t size);
		virtual ~GLReadbackBuffer() = default;

		virtual void Readback(Ref<ShaderBuffer> buffer, uint32_t size) override;
		virtual void GetData(void* dest, uint32_t size) override;
	};
}
