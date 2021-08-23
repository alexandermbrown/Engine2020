#pragma once

#include "Shader.h"
#include "IndirectBuffer.h"
#include "ShaderBuffer.h"
#include "UniformBuffer.h"
#include "Lithium/Utility/Time.h"
#include "glm/glm.hpp"

namespace Li
{
	struct EmitterProps
	{
		uint32_t Count;
		float EmitRate;
		glm::vec2 LifeSpan;
	};

	class ParticleEmitterWI
	{
	public:
		ParticleEmitterWI(const EmitterProps& props);

		void Update(Li::Duration::us dt, const glm::mat4& transform);
		void Draw();

	private:
		uint32_t m_MaxCount;
		float m_EmitCount;
		float m_EmitRate;
		glm::vec2 m_LifeSpan;
		static constexpr int ReadbackBufferCount = 4;

		Ref<ShaderBuffer> m_ParticleBuffer;
		Ref<ShaderBuffer> m_AliveList[2];
		Ref<ShaderBuffer> m_DeadList;
		Ref<ShaderBuffer> m_DistanceBuffer;
		Ref<ShaderBuffer> m_CounterBuffer;

		// Indirect Argument Buffers.
		Ref<IndirectBuffer> m_ComputeIAB;
		Ref<IndirectBuffer> m_DrawIAB;

		Ref<Shader> m_ShaderUpdateBegin;
		Ref<Shader> m_ShaderEmit;
		Ref<Shader> m_ShaderSimulate;
		Ref<Shader> m_ShaderUpdateEnd;
		Ref<Shader> m_ShaderRender;

		Ref<ShaderBuffer> m_CountReadback[ReadbackBufferCount];
	};
}
