#include "lipch.h"
#include "GPUSort.h"

#include "ShaderInterop/GPUSortSI.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace Li
{
	GPUSort::GPUSort()
		: m_SortBegin(ResourceManager::GetShader("shader_gpusort_begin")),
		m_Sort       (ResourceManager::GetShader("shader_gpusort_sort")),
		m_SortInner  (ResourceManager::GetShader("shader_gpusort_sort_inner")),
		m_SortStep   (ResourceManager::GetShader("shader_gpusort_sort_step"))
	{
		m_IndirectBuffer = IndirectBuffer::Create(sizeof(IndirectDispatchArgs), IndirectTarget::Compute);
		m_SortUB = UniformBuffer::Create(LI_CB_GETBINDSLOT(SortCB), sizeof(SortCB));
	}

	void GPUSort::Sort(uint32_t max_count,
		const Ref<ShaderBuffer>& comparison_buffer_read,
		const Ref<ShaderBuffer>& counter_buffer_read,
		uint32_t counter_read_offset,
		const Ref<ShaderBuffer>& index_buffer_write)
	{
		GraphicsContext* context = Application::Get().GetWindow().GetContext();

		SortCB sc;
		sc.counterReadOffset = counter_read_offset;
		m_SortUB->SetData(&sc);
		m_SortUB->BindBase();
		m_SortUB->Bind(ShaderType::Compute);

		context->UnbindUAVs(0, 8);
		
		// Init sorting arguments
		{
			m_SortBegin->Bind();
			counter_buffer_read->BindBase(0);
			counter_buffer_read->BindSRV(ShaderType::Compute, 0);
			m_IndirectBuffer->Bind(3);

			context->ShaderStorageBarrier();
			context->DispatchCompute(1, 1, 1);
			context->CommandBarrier();

			context->UnbindUAVs(3, 1);
		}

		counter_buffer_read   ->BindBase(0);
		comparison_buffer_read->BindBase(1);
		index_buffer_write    ->BindBase(2);

		counter_buffer_read   ->BindSRV(ShaderType::Compute, 0);
		comparison_buffer_read->BindSRV(ShaderType::Compute, 1);
		index_buffer_write    ->BindUAV(2);

		// Initial sorting
		bool done = true;
		{
			unsigned int num_thread_groups = ((max_count - 1) >> 9) + 1;

			if (num_thread_groups > 1)
				done = false;

			m_Sort->Bind();
			m_IndirectBuffer->DispatchComputeIndirect(0);

			context->ShaderStorageBarrier();
		}

		int presorted = 512;
		while (!done)
		{
			done = true;

			m_SortStep->Bind();

			uint32_t num_thread_groups = 0;

			if (max_count > (uint32_t)presorted)
			{
				if (max_count > (uint32_t)presorted * 2)
					done = false;

				uint32_t pow2 = presorted;
				while (pow2 < max_count)
					pow2 *= 2;
				num_thread_groups = pow2 >> 9;
			}

			uint32_t n_merge_size = presorted * 2;
			for (uint32_t n_merge_sub_size = n_merge_size >> 1; n_merge_sub_size > 256; n_merge_sub_size = n_merge_sub_size >> 1)
			{
				SortCB sc;
				sc.job_params.x = n_merge_sub_size;
				if (n_merge_sub_size == n_merge_size >> 1)
				{
					sc.job_params.y = (2 * n_merge_sub_size - 1);
					sc.job_params.z = -1;
				}
				else
				{
					sc.job_params.y = n_merge_sub_size;
					sc.job_params.z = 1;
				}
				sc.counterReadOffset = counter_read_offset;

				m_SortUB->SetData(&sc);
				m_SortUB->BindBase();
				m_SortUB->Bind(ShaderType::Compute);

				context->DispatchCompute(num_thread_groups, 1, 1);
				context->ShaderStorageBarrier();
			}

			m_SortInner->Bind();
			context->DispatchCompute(num_thread_groups, 1, 1);
			context->ShaderStorageBarrier();

			presorted *= 2;
		}

		context->UnbindResources(0, 2);
		context->UnbindUAVs(2, 1);
	}
}
