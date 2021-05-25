#pragma once

#include "Lithium/Renderer/StagingBuffer.h"

namespace Li
{
	class OpenGLStagingBuffer : public StagingBuffer
	{
	public:
		OpenGLStagingBuffer(uint32_t size);
		virtual ~OpenGLStagingBuffer();

	private:
		uint32_t m_RendererID;
	};
}