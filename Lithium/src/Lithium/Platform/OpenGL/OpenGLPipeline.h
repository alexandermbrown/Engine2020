#pragma once

#include "Lithium/Renderer/Pipeline.h"

#include <vector>

namespace Li 
{
	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(const Pipeline::Spec& spec);
		virtual ~OpenGLPipeline();

		virtual void Bind(const BindArray& vertex_buffers) const override;
	private:
		uint32_t m_VAO;
	};
}
