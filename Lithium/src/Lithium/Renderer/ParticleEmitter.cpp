#include "lipch.h"
#include "ParticleEmitter.h"

#include "Renderer.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace Li
{
	ParticleEmitter::ParticleEmitter(const EmitterProps& props)
		: m_MaxCount(props.MaxCount), m_EmitRate(props.EmitRate), m_Continuous(props.Continuous),
		m_EmitCount(0.0f), m_BurstCount(0.0f),
		m_BurstTimer(Duration::us(0), true),
		m_ShaderUpdateBegin(ResourceManager::GetShader("shader_emitter_update_begin")),
		m_ShaderEmit       (ResourceManager::GetShader("shader_emitter_emit")),
		m_ShaderSimulate   (ResourceManager::GetShader("shader_emitter_simulate")),
		m_ShaderUpdateEnd  (ResourceManager::GetShader("shader_emitter_update_end")),
		m_ShaderDraw       (ResourceManager::GetShader("shader_emitter_draw"))
	{
		const GraphicsFactory* factory = GraphicsFactory::Get();
		m_EmitterUB = factory->MakeUniformBuffer(LI_CB_GETBINDSLOT(EmitterCB), sizeof(EmitterCB));

		m_Uniforms.u_LifeSpan        = props.LifeSpan;
		m_Uniforms.u_SpeedRange      = props.SpeedRange;
		m_Uniforms.u_Scale           = props.ParticleScale;
		m_Uniforms.u_Rotation = { props.InitialAngle, props.AngularVelocity };
		m_Uniforms.u_RelativeToWorld = props.RelativeToWorld;
		m_Uniforms.u_EmitVolume      = props.EmitVolume;
		m_Uniforms.u_Acceleration    = props.Acceleration;
		m_Uniforms.u_AirResistance   = props.AirResistance;
		for (int i = 0; i < LI_GRAPH_NODE_COUNT_MAX; i++)
		{
			m_Uniforms.u_ScaleGraph[i] = glm::vec4(props.ScaleGraph[i], 0.0f, 0.0f);
			m_Uniforms.u_RedGraph  [i] = glm::vec4(props.RedGraph  [i], 0.0f, 0.0f);
			m_Uniforms.u_GreenGraph[i] = glm::vec4(props.GreenGraph[i], 0.0f, 0.0f);
			m_Uniforms.u_BlueGraph [i] = glm::vec4(props.BlueGraph [i], 0.0f, 0.0f);
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
		
		m_Particles = factory->MakeShaderBuffer(particles.data(), m_MaxCount * sizeof(Particle), sizeof(Particle), ShaderBufferType::Structured);

		m_AliveList[0] = factory->MakeShaderBuffer(nullptr, m_MaxCount * sizeof(uint32_t), sizeof(uint32_t), ShaderBufferType::Structured);
		m_AliveList[1] = factory->MakeShaderBuffer(nullptr, m_MaxCount * sizeof(uint32_t), sizeof(uint32_t), ShaderBufferType::Structured);

		std::vector<uint32_t> dead_indices(m_MaxCount);
		for (uint32_t i = 0; i < m_MaxCount; i++)
			dead_indices[i] = i;
		m_DeadList = factory->MakeShaderBuffer(dead_indices.data(), m_MaxCount * sizeof(uint32_t), sizeof(uint32_t), ShaderBufferType::Structured);

		m_DistanceBuffer = factory->MakeShaderBuffer(nullptr, m_MaxCount * sizeof(float), sizeof(float), ShaderBufferType::Structured);

		ParticleCounters counters;
		counters.alive_count = 0;
		counters.dead_count = m_MaxCount;
		counters.real_emit_count = 0;
		counters.alive_count_after_sim = 0;

		m_CounterBuffer = factory->MakeShaderBuffer(&counters, sizeof(counters), sizeof(uint32_t), ShaderBufferType::Raw);
		m_ReadbackBuffer = factory->MakeReadbackBuffer(sizeof(counters));

		m_ComputeIAB = factory->MakeIndirectBuffer(sizeof(IndirectDispatchArgs) * 2, IndirectTarget::Compute);
		m_DrawIAB = factory->MakeIndirectBuffer(sizeof(IndirectDrawInstancedArgs), IndirectTarget::Draw);
	}

	void ParticleEmitter::Update(Li::Duration::us dt, const glm::mat4& transform)
	{
		if (!m_Continuous && m_BurstTimer.Update(dt))
			return;

		GraphicsContext* context = Application::Get().GetWindow().GetContext();

		std::swap(m_AliveList[0], m_AliveList[1]);

		float delta = std::min(Li::Duration::Cast<Li::Duration::fsec>(dt).count(), 0.2f);

		m_EmitCount = std::max(0.0f, m_EmitCount - std::floor(m_EmitCount));
		if (m_Continuous)
		{
			m_EmitCount += m_EmitRate * delta;
		}
		else
		{
			float emit_count = std::min(m_BurstCount, m_EmitRate * delta);
			m_BurstCount -= emit_count;
			m_EmitCount += emit_count;
		}

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
		m_DistanceBuffer->BindBase(6);

		m_Particles->BindUAV(0);
		m_AliveList[0]->BindUAV(1);
		m_AliveList[1]->BindUAV(2);
		m_DeadList->BindUAV(3);
		m_CounterBuffer->BindUAV(4);
		m_DistanceBuffer->BindUAV(6);

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

		Renderer::GetSorter()->Sort(m_MaxCount, m_DistanceBuffer, m_CounterBuffer,
			PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION, m_AliveList[1]);

		context->ShaderStorageBarrier();

		m_CounterBuffer->BindBase(4);
		m_CounterBuffer->BindUAV(4);

		m_ShaderUpdateEnd->Bind();
		m_DrawIAB->Bind(6);
		context->DispatchCompute(1, 1, 1);

		context->UnbindUAVs(0, 5);
	}

	void ParticleEmitter::Draw(const Ref<Texture2D>& texture)
	{
		if (!m_Continuous && m_BurstTimer.IsCompleted())
			return;

		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		
		m_ShaderDraw->Bind();
		Renderer::GetViewProjBuffer()->Bind(ShaderType::Vertex);
		m_Particles->BindBase(1);
		m_Particles->BindSRV(ShaderType::Vertex, 1);
		m_AliveList[1]->BindBase(2);
		m_AliveList[1]->BindSRV(ShaderType::Vertex, 2);
		m_EmitterUB->BindBase();
		m_EmitterUB->Bind(ShaderType::Vertex);

		texture->Bind(0);

		context->UnbindVertexArray();

		context->ShaderStorageBarrier();
		context->CommandBarrier();
		context->SetDrawMode(DrawMode::Triangles);
		m_DrawIAB->DrawInstancedIndirect(DrawMode::Triangles, DRAW_IAB_OFFSET_DRAWPARTICLES);

		context->UnbindResources(1, 2);
	}

	void ParticleEmitter::Burst(int count)
	{
		float active_time = (float)count / m_EmitRate + m_Uniforms.u_LifeSpan.y;
		Duration::us delay = Duration::Cast<Duration::us>(Duration::fsec(active_time));

		m_BurstTimer.Reset();
		m_BurstTimer.SetDelay(delay);

		m_BurstCount += (float)count;
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
