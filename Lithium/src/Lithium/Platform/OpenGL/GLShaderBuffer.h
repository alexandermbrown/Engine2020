#pragma once

#include "GLBuffer.h"
#include "Lithium/Renderer/ShaderBuffer.h"

namespace Li
{
	class GLShaderBuffer : public ShaderBuffer, public GLBuffer
	{
	public:
		GLShaderBuffer(const void* data, uint32_t size);
		virtual ~GLShaderBuffer() = default;

		virtual void BindBase(uint32_t slot) const override;
		virtual void BindSRV(ShaderType type, uint32_t slot) const override {};
		virtual void BindUAV(uint32_t slot) const override {};
	};
}
