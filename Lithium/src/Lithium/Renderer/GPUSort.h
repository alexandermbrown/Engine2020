#pragma once

#include "IndirectBuffer.h"
#include "ShaderBuffer.h"
#include "UniformBuffer.h"
#include "Shader.h"

namespace Li
{
	class GPUSort
	{
	public:
		GPUSort();

		void Sort(uint32_t max_count,
			const Ref<ShaderBuffer>& comparison_buffer_read,
			const Ref<ShaderBuffer>& counter_buffer_read,
			uint32_t counter_read_offset,
			const Ref<ShaderBuffer>& index_buffer_write);

	private:
		Ref<IndirectBuffer> m_IndirectBuffer;
		Ref<UniformBuffer> m_SortUB;
		Ref<Shader> m_SortBegin;
		Ref<Shader> m_Sort;
		Ref<Shader> m_SortInner;
		Ref<Shader> m_SortStep;
	};
}
