#include "lipch.h"
#include "ParticleEmitter.h"

#include "Renderer.h"
#include "ShaderInterop/ParticleEmitterSI.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

namespace Li
{
	ParticleEmitter::ParticleEmitter(const EmitterProps& props)
		: m_MaxCount(props.Count), m_EmitCount(0.0f), m_EmitRate(props.EmitRate), m_LifeSpan(props.LifeSpan),
		m_ShaderUpdateBegin(ResourceManager::GetShader("shader_emitter_update_begin")),
		m_ShaderSimulate(ResourceManager::GetShader("shader_emitter_simulate")),
		m_ShaderUpdateEnd(ResourceManager::GetShader("shader_emitter_update_end")),
		m_ShaderDraw(ResourceManager::GetShader("shader_emitter_draw"))
	{
		m_MaxCount = THREADCOUNT_SIMULATION;

		std::vector<Particle> particles(m_MaxCount);
		for (Particle& particle : particles)
		{
			Random& rand = Application::Get().GetRandom();
			
			particle.position = { 0.0f, 0.0f, 0.0f };
			float speed = rand.UniformFloat(3.0f, 5.0f);
			float angle = rand.UniformFloat(0.0f, 2.0f * (float)M_PI);
			particle.velocity = {
				speed * std::cos(angle),
				speed * std::sin(angle), 0.0f
			};

			particle.max_life = rand.UniformFloat(props.LifeSpan.x, props.LifeSpan.y);
			particle.life = particle.max_life;

			particle.color = {
				rand.UniformFloat(0.4f, 0.9f),
				rand.UniformFloat(0.4f, 0.9f),
				rand.UniformFloat(0.4f, 0.9f),
				0.7f
			};
		}

		m_ParticleBuffer = ShaderBuffer::Create(particles.data(), m_MaxCount * sizeof(Particle), sizeof(Particle), ShaderBufferType::Structured);

		m_ComputeIAB = IndirectBuffer::Create(sizeof(IndirectDispatchArgs), IndirectTarget::Compute);
		m_DrawIAB = IndirectBuffer::Create(sizeof(IndirectDrawInstancedArgs), IndirectTarget::Draw);
	}

	void ParticleEmitter::Update(Li::Duration::us dt, const glm::mat4& transform)
	{
		EmitterCB emitter;
		emitter.u_EmitterTransform = transform;
		emitter.u_EmitCount = static_cast<uint32_t>(m_EmitCount);
		emitter.u_EmitterRandomness = Application::Get().GetRandom().UniformFloat(0.0f, 1.0f);
		emitter.u_ParticleLifeSpan = m_LifeSpan;

		Ref<UniformBuffer> emitter_buffer = Renderer::GetEmitterBuffer();
		emitter_buffer->SetData(&emitter);
		emitter_buffer->Bind(ShaderType::Compute);
		Renderer::GetFrameUniformBuffer()->Bind(ShaderType::Compute);

		m_ParticleBuffer->BindBase(0);
		m_ParticleBuffer->BindUAV(0);

		GraphicsContext* context = Application::Get().GetWindow().GetContext();

		m_ComputeIAB->Bind(5);
		m_ShaderUpdateBegin->Bind();
		context->DispatchCompute(1, 1, 1);

		m_ShaderSimulate->Bind();
		m_ComputeIAB->DispatchComputeIndirect(COMPUTE_IAB_OFFSET_DISPATCHSIMULATION);
		
		m_ShaderUpdateEnd->Bind();
		m_DrawIAB->Bind(6);
		context->DispatchCompute(1, 1, 1);

		context->UnbindUAVs(0, 1);
	}

	void ParticleEmitter::Draw()
	{
		m_ShaderDraw->Bind();
		Renderer::GetViewProjBuffer()->Bind(ShaderType::Vertex);
		m_ParticleBuffer->BindBase(1);
		m_ParticleBuffer->BindSRV(ShaderType::Vertex, 1);

		GraphicsContext* context = Application::Get().GetWindow().GetContext();
		context->UnbindVertexArray();

		context->SetDrawMode(DrawMode::Triangles);
		m_DrawIAB->DrawInstancedIndirect(DrawMode::Triangles, DRAW_IAB_OFFSET_DRAWPARTICLES);

		context->UnbindResources(1, 1);
	}
}
