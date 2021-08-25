#pragma once

#include "OpenGLBuffer.h"
#include "Lithium/Renderer/ShaderBuffer.h"

namespace Li
{
	class OpenGLShaderBuffer : public ShaderBuffer, public OpenGLBuffer
	{
	public:
		OpenGLShaderBuffer(const void* data, uint32_t size);
		virtual ~OpenGLShaderBuffer() = default;

		virtual void BindBase(uint32_t slot) const override;
		virtual void BindSRV(ShaderType type, uint32_t slot) const override {};
		virtual void BindUAV(uint32_t slot) const override {};
	};
}
