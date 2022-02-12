#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include <memory>
#include <array>

namespace Li 
{
	class Pipeline
	{
	public:
		static constexpr int MaxVertexBufferCount = 8;
		struct Spec
		{
			unsigned int VertexBufferCount;
			std::array<VertexBufferLayout, MaxVertexBufferCount> Layouts;
			Ref<Shader> ShaderRef;
		};

		Pipeline(const Pipeline::Spec& spec)
			: m_Spec(spec)
		{
			LI_CORE_ASSERT(spec.VertexBufferCount < MaxVertexBufferCount, "Vertex buffer count too large!");
		}

		virtual ~Pipeline() = default;

		using BindArray = std::array<Ref<VertexBuffer>, MaxVertexBufferCount>;
		virtual void Bind(const BindArray& vertex_buffers) const = 0;
		const Spec& GetSpec() const { return m_Spec; }

	protected:
		Pipeline::Spec m_Spec;
	};
}
