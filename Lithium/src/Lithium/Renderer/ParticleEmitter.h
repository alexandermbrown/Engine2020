#pragma once

#include "IndirectBuffer.h"
#include "ReadbackBuffer.h"
#include "Shader.h"
#include "ShaderBuffer.h"
#include "Lithium/Utility/Time.h"
#include "glm/glm.hpp"

namespace Li
{
	struct EmitterProps
	{
		uint32_t MaxCount;
		float EmitRate;
		glm::vec2 LifeSpan;
		glm::vec2 SpeedRange;
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
		EmitterProps m_Props;
		float m_EmitCount;

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
