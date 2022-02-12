#pragma once

#include "Lithium/Renderer/Pipeline.h"

#include <vector>

namespace Li 
{
	class GLPipeline : public Pipeline
	{
	public:
		GLPipeline(const Pipeline::Spec& spec);
		virtual ~GLPipeline();

		virtual void Bind(const BindArray& vertex_buffers) const override;
	private:
		uint32_t m_VAO;
	};
}
