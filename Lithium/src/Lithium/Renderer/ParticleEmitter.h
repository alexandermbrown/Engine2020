#pragma once

#include "IndirectBuffer.h"
#include "ReadbackBuffer.h"
#include "Shader.h"
#include "ShaderBuffer.h"
#include "ShaderInterop/ParticleEmitterSI.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Utility/Time.h"
#include "glm/glm.hpp"

namespace Li
{
	struct EmitterProps
	{
		// Emmision.
		uint32_t MaxCount;
		float EmitRate;
		bool Continuous;
		bool RelativeToWorld;
		// XYZ scale of spawn volume around origin.
		// Particles will spawn randomly in this area.
		glm::vec3 EmitVolume;

		// Simulation.
		glm::vec2 LifeSpan;
		glm::vec2 SpeedRange;
		glm::vec3 ParticleScale;
		// Inital angle and angular velocity are (min, max).
		glm::vec2 InitialAngle;
		glm::vec2 AngularVelocity;
		glm::vec3 Acceleration;
		
		// First 3 floats is scale xyz, last float is time fraction.
		glm::vec2 ScaleGraph[LI_GRAPH_NODE_COUNT_MAX] {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		// Visual.
		// // First float is alpha, last float is time fraction.
		glm::vec2 RedGraph[LI_GRAPH_NODE_COUNT_MAX] {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f }
		};
		// First float is alpha, last float is time fraction.
		glm::vec2 BlueGraph[LI_GRAPH_NODE_COUNT_MAX] {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f }
		};
		// First float is alpha, last float is time fraction.
		glm::vec2 GreenGraph[LI_GRAPH_NODE_COUNT_MAX] {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f }
		};
		// First float is alpha, last float is time fraction.
		glm::vec2 AlphaGraph[LI_GRAPH_NODE_COUNT_MAX] {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f }
		};
	};

	class ParticleEmitter
	{
	public:
		ParticleEmitter(const EmitterProps& props);

		void Update(Li::Duration::us dt, const glm::mat4& transform);
		void Draw(const Ref<Texture2D>& texture);

		inline void Burst(int count)
		{
			m_BurstCount += (float)count;
		}

		// Warning: May impact performance.
		void PrintDebug(const char* label);

	private:
		//EmitterProps m_Props;
		const uint32_t m_MaxCount;
		float m_EmitCount;
		float m_EmitRate;

		bool m_Continuous;
		float m_BurstCount;

		EmitterCB m_Uniforms;
		Ref<UniformBuffer> m_EmitterUB;
		Ref<ShaderBuffer> m_Particles;
		Ref<ShaderBuffer> m_AliveList[2];
		Ref<ShaderBuffer> m_DeadList;
		Ref<ShaderBuffer> m_CounterBuffer;
		Ref<ReadbackBuffer> m_ReadbackBuffer;

		Ref<Shader> m_ShaderUpdateBegin;
		Ref<Shader> m_ShaderEmit;
		Ref<Shader> m_ShaderSimulate;
		Ref<Shader> m_ShaderUpdateEnd;
		Ref<Shader> m_ShaderDraw;

		Ref<IndirectBuffer> m_ComputeIAB;
		Ref<IndirectBuffer> m_DrawIAB;
	};
}
