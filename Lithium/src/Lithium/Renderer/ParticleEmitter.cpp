#include "lipch.h"
#include "ParticleEmitter.h"

#include "Renderer.h"
#include "ShaderInterop/ParticleEmitterSI.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include <math.h>

namespace Li
{
	ParticleEmitter::ParticleEmitter(const EmitterProps& props)
		: m_MaxCount(props.Count), m_EmitCount(0.0f), m_EmitRate(props.EmitRate), m_LifeSpan(props.LifeSpan),
		m_EmitterUB         (Renderer::GetEmitterUB()),
		m_ShaderUpdateBegin (ResourceManager::GetShader("shader_emitter_update_begin")),
		m_ShaderEmit        (ResourceManager::GetShader("shader_emitter_emit")),
		m_ShaderSimulate    (ResourceManager::GetShader("shader_emitter_simulate")),
		m_ShaderUpdateEnd   (ResourceManager::GetShader("shader_emitter_update_end")),
		m_ShaderRender      (ResourceManager::GetShader("shader_emitter_render"))
	{
		m_ParticleBuffer = ShaderBuffer::Create(nullptr, m_MaxCount * sizeof(Particle), sizeof(Particle), ShaderBufferType::Structured);
		m_AliveList[0] = ShaderBuffer::Create(nullptr, m_MaxCount * sizeof(uint32_t), sizeof(uint32_t), ShaderBufferType::Structured);
		m_AliveList[1] = ShaderBuffer::Create(nullptr, m_MaxCount * sizeof(uint32_t), sizeof(uint32_t), ShaderBufferType::Structured);

		std::vector<uint32_t> dead_indices(m_MaxCount);
		for (uint32_t i = 0; i < m_MaxCount; i++)
			dead_indices[i] = i;
		m_DeadList = ShaderBuffer::Create(dead_indices.data(), m_MaxCount * sizeof(uint32_t), sizeof(uint32_t), ShaderBufferType::Structured);

		ParticleCounters counters;
		counters.alive_count = 0;
		counters.dead_count = m_MaxCount;
		counters.real_emit_count = 0;
		counters.alive_count_after_sim = 0;

		m_CounterBuffer = ShaderBuffer::Create(&counters, sizeof(counters), sizeof(counters), ShaderBufferType::Raw);

		m_ComputeIAB = IndirectBuffer::Create(sizeof(IndirectDispatchArgs) * 2, sizeof(IndirectDispatchArgs) * 2, IndirectTarget::Compute);
		m_DrawIAB = IndirectBuffer::Create(sizeof(IndirectDrawArgsInstanced), sizeof(IndirectDrawArgsInstanced), IndirectTarget::Draw);
	}

	void ParticleEmitter::Update(Li::Duration::us dt, const glm::mat4& transform)
	{
		// Update CPU

		// Reset emit count to 0 but keep decimal.
		m_EmitCount = std::max(0.0f, m_EmitCount - std::floorf(m_EmitCount));
		m_EmitCount += m_EmitRate * Li::Duration::Cast<Li::Duration::fsec>(dt).count();

		// Update GPU

		EmitterCB cb;
		cb.u_EmitterTransform = transform;
		cb.u_EmitCount = static_cast<uint32_t>(m_EmitCount);
		cb.u_EmitterRandomness = Application::Get().GetRandom().UniformFloat(0.0f, 1.0f);
		cb.u_ParticleLifeSpan = m_LifeSpan;
		m_EmitterUB->SetData(&cb);

		m_EmitterUB->Bind(ShaderType::Compute);

		m_ParticleBuffer->BindBase(0);
		m_AliveList[0]  ->BindBase(1);
		m_AliveList[1]  ->BindBase(2);
		m_DeadList      ->BindBase(3);
		m_CounterBuffer ->BindBase(4);

		m_ParticleBuffer->BindUAV(0);
		m_AliveList[0]  ->BindUAV(1);
		m_AliveList[1]  ->BindUAV(2);
		m_DeadList      ->BindUAV(3);
		m_CounterBuffer ->BindUAV(4);

		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		m_ShaderUpdateBegin->Bind();
		m_ComputeIAB->Bind(5);
		context->DispatchCompute(1, 1, 1);
		
		context->ShaderStorageBarrier();

		m_ShaderEmit->Bind();
		m_ComputeIAB->DispatchComputeIndirect(COMPUTE_IAB_OFFSET_DISPATCHEMIT);

		context->ShaderStorageBarrier();
		
		m_ShaderSimulate->Bind();
		m_ComputeIAB->DispatchComputeIndirect(COMPUTE_IAB_OFFSET_DISPATCHSIMULATION);

		context->ShaderStorageBarrier();

		m_ShaderUpdateEnd->Bind();
		context->DispatchCompute(1, 1, 1);

		context->ShaderStorageBarrier();

		// Unbind UAVs.
		context->UnbindUAVs(0, 5);
	}

	void ParticleEmitter::Draw()
	{
		m_ShaderRender->Bind();
		Renderer::GetViewProjUniformBuffer()->Bind(ShaderType::Vertex);
		m_ParticleBuffer->BindSRV(ShaderType::Vertex, 1);
		m_AliveList[1]->BindSRV(ShaderType::Vertex, 2);
		m_CounterBuffer->BindSRV(ShaderType::Compute, 4);
		m_DrawIAB->Bind(5);
		m_DrawIAB->DrawInstancedIndirect(DrawMode::Triangles, DRAW_IAB_OFFSET_DRAWPARTICLES);

		Application::Get().GetWindow().GetContext()->UnbindResources(1, 4);
	}
}
