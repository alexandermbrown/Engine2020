#pragma once

#include "Lithium/Renderer/RendererEnums.h"

namespace Li
{
	class GLRenderbuffer
	{
	public:
		GLRenderbuffer(uint32_t width, uint32_t height);
		virtual ~GLRenderbuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void Resize(uint32_t width, uint32_t height);
		virtual void AttachToFramebuffer() const;

	private:
		uint32_t m_RendererID;
	};
}
