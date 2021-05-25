#pragma once

#include "Lithium/Renderer/ShaderBuffer.h"

namespace Li
{
	class OpenGLShaderBuffer : public ShaderBuffer
	{
	public:
		OpenGLShaderBuffer(const void* data, uint32_t size);
		virtual ~OpenGLShaderBuffer();

		virtual void BindBase(uint32_t slot) const override;
		virtual void BindSRV(ShaderType type, uint32_t slot) const override {};
		virtual void BindUAV(uint32_t slot) const override {};

	private:
		uint32_t m_RendererID;
	};
}
