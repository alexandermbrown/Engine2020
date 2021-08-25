#pragma once

#include "OpenGLBuffer.h"
#include "Lithium/Renderer/ReadbackBuffer.h"

namespace Li
{
	class OpenGLReadbackBuffer : public ReadbackBuffer, public OpenGLBuffer
	{
	public:
		OpenGLReadbackBuffer(uint32_t size);
		virtual ~OpenGLReadbackBuffer() = default;

		virtual void Readback(Ref<ShaderBuffer> buffer, uint32_t size) override;
		virtual void GetData(void* dest, uint32_t size) override;
	};
}
