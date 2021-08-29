#pragma once

#include "IndirectBuffer.h"
#include "ReadbackBuffer.h"
#include "Shader.h"
#include "ShaderBuffer.h"
#include "ShaderInterop/ParticleEmitterSI.h"
#include "Lithium/Utility/Time.h"
#include "glm/glm.hpp"

namespace Li
{
	struct EmitterProps
	{
		uint32_t MaxCount;
		float EmitRate;
		bool RelativeToWorld;
		glm::vec2 LifeSpan;
		glm::vec2 SpeedRange;
		glm::vec3 ParticleScale;
		// Inital angle and angular velocity are (min, max).
		glm::vec2 InitialAngle;
		glm::vec2 AngularVelocity;
		// First 3 floats is scale xyz, last float is time fraction.
		glm::vec2 ScaleGraph[LI_GRAPH_NODE_COUNT_MAX];
		// First float is alpha, last float is time fraction.
		glm::vec2 AlphaGraph[LI_GRAPH_NODE_COUNT_MAX];
		// First 3 floats are color rgb, last float is time fraction.
		//glm::vec4 ColorGraph[LI_GRAPH_NODE_COUNT_MAX];
	};

	class ParticleEmitter
	{
	public:
		ParticleEmitter(const EmitterProps& props);

		void Update(Li::Duration::us dt, const glm::mat4& transform);
		void Draw();
		// Warning: May impact performance.
		void PrintDebug(const char* label);

	private:
		//EmitterProps m_Props;
		const uint32_t m_MaxCount;
		float m_EmitCount;
		float m_EmitRate;

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
