#include "lipch.h"
#include "ParticleEmitter.h"

#include "Renderer.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace Li
{
	ParticleEmitter::ParticleEmitter(const EmitterProps& props)
		: m_MaxCount(props.MaxCount), m_EmitRate(props.EmitRate), m_EmitCount(0.0f),
		m_ShaderUpdateBegin(ResourceManager::GetShader("shader_emitter_update_begin")),
		m_ShaderEmit(ResourceManager::GetShader("shader_emitter_emit")),
		m_ShaderSimulate(ResourceManager::GetShader("shader_emitter_simulate")),
		m_ShaderUpdateEnd(ResourceManager::GetShader("shader_emitter_update_end")),
		m_ShaderDraw(ResourceManager::GetShader("shader_emitter_draw"))
	{
		m_EmitterUB = UniformBuffer::Create(LI_CB_GETBINDSLOT(EmitterCB), sizeof(EmitterCB));

		m_Uniforms.u_LifeSpan = props.LifeSpan;
		m_Uniforms.u_SpeedRange = props.SpeedRange;
		m_Uniforms.u_Scale = props.ParticleScale;
		for (int i = 0; i < LI_GRAPH_NODE_COUNT_MAX; i++)
		{
			m_Uniforms.u_ScaleGraph[i] = glm::vec4(props.ScaleGraph[i], 0.0f, 0.0f);
			m_Uniforms.u_AlphaGraph[i] = glm::vec4(props.AlphaGraph[i], 0.0f, 0.0f);
		}

		std::vector<Particle> particles(m_MaxCount);
		for (Particle& particle : particles)
		{
			Random& rand = Application::Get().GetRandom();
			particle.position = { 0.0f, 0.0f, 0.0f };
			particle.velocity = { 0.0f, 0.0f, 0.0f };
			particle.life_left = 0.0f;
			particle.color = { 1.0f, 0.0f, 0.0f, 1.0f };
		}

		m_Particles = ShaderBuffer::Create(particles.data(), m_MaxCount * sizeof(Particle), sizeof(Particle), ShaderBufferType::Structured);

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

		m_CounterBuffer = ShaderBuffer::Create(&counters, sizeof(counters), sizeof(uint32_t), ShaderBufferType::Raw);
		m_ReadbackBuffer = ReadbackBuffer::Create(sizeof(counters));

		m_ComputeIAB = IndirectBuffer::Create(sizeof(IndirectDispatchArgs) * 2, IndirectTarget::Compute);
		m_DrawIAB = IndirectBuffer::Create(sizeof(IndirectDrawInstancedArgs), IndirectTarget::Draw);
	}

	void ParticleEmitter::Update(Li::Duration::us dt, const glm::mat4& transform)
	{
		GraphicsContext* context = Application::Get().GetWindow().GetContext();

		std::swap(m_AliveList[0], m_AliveList[1]);

		float delta = std::min(Li::Duration::Cast<Li::Duration::fsec>(dt).count(), 0.2f);

		m_EmitCount = std::max(0.0f, m_EmitCount - std::floorf(m_EmitCount));
		m_EmitCount += m_EmitRate * delta;

		m_Uniforms.u_EmitterTransform = transform;
		m_Uniforms.u_EmitCount = static_cast<uint32_t>(m_EmitCount);
		m_Uniforms.u_EmitterRandomness = Application::Get().GetRandom().UniformFloat(0.0f, 1.0f);

		m_EmitterUB->SetData(&m_Uniforms);
		m_EmitterUB->BindBase();
		m_EmitterUB->Bind(ShaderType::Compute);
		Renderer::GetFrameUniformBuffer()->Bind(ShaderType::Compute);

		m_Particles->BindBase(0);
		m_AliveList[0]->BindBase(1);
		m_AliveList[1]->BindBase(2);
		m_DeadList->BindBase(3);
		m_CounterBuffer->BindBase(4);

		m_Particles->BindUAV(0);
		m_AliveList[0]->BindUAV(1);
		m_AliveList[1]->BindUAV(2);
		m_DeadList->BindUAV(3);
		m_CounterBuffer->BindUAV(4);

		context->ShaderStorageBarrier();

		m_ComputeIAB->Bind(5);
		m_ShaderUpdateBegin->Bind();
		context->DispatchCompute(1, 1, 1);

		context->ShaderStorageBarrier();
		context->CommandBarrier();

		m_ShaderEmit->Bind();
		m_ComputeIAB->DispatchComputeIndirect(COMPUTE_IAB_OFFSET_DISPATCHEMIT);

		context->ShaderStorageBarrier();

		m_ShaderSimulate->Bind();
		m_ComputeIAB->DispatchComputeIndirect(COMPUTE_IAB_OFFSET_DISPATCHSIMULATION);
		
		context->ShaderStorageBarrier();

		m_ShaderUpdateEnd->Bind();
		m_DrawIAB->Bind(6);
		context->DispatchCompute(1, 1, 1);

		context->UnbindUAVs(0, 5);
	}

	void ParticleEmitter::Draw()
	{
		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		
		m_ShaderDraw->Bind();
		Renderer::GetViewProjBuffer()->Bind(ShaderType::Vertex);
		m_Particles->BindBase(1);
		m_Particles->BindSRV(ShaderType::Vertex, 1);
		m_AliveList[1]->BindBase(2);
		m_AliveList[1]->BindSRV(ShaderType::Vertex, 2);

		context->UnbindVertexArray();

		context->ShaderStorageBarrier();
		context->CommandBarrier();
		context->SetDrawMode(DrawMode::Triangles);
		m_DrawIAB->DrawInstancedIndirect(DrawMode::Triangles, DRAW_IAB_OFFSET_DRAWPARTICLES);

		context->UnbindResources(1, 2);
	}

	void ParticleEmitter::PrintDebug(const char* label)
	{
		ParticleCounters counters;
		m_ReadbackBuffer->Readback(m_CounterBuffer, sizeof(ParticleCounters));
		m_ReadbackBuffer->GetData(&counters, sizeof(counters));
		Li::Log::CoreTrace(
			"{}: alive {}, dead {}, real_emit {}, alive_after_sim {}, Emit Count: {}",
			label, counters.alive_count, counters.dead_count, counters.real_emit_count,
			counters.alive_count_after_sim, m_EmitCount
		);
	}
}
