#pragma once

#include "Buffer.h"

namespace Li
{
	class Model
	{
	public:
		Model(const std::string& path);
		
	private:
		std::string m_Name;
		Ref<VertexBuffer> m_Vertices;
		Ref<IndexBuffer> m_Indices;

		friend class Renderer;
	};
}
