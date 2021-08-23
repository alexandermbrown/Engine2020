#pragma once

#include "Shader.h"
#include "ShaderBuffer.h"
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

	class ParticleEmitter
	{
	public:
		ParticleEmitter(const EmitterProps& props);

		void Update(Li::Duration::us dt, const glm::mat4& transform);
		void Draw();

	private:
		uint32_t m_MaxCount;
		float m_EmitCount;
		float m_EmitRate;
		glm::vec2 m_LifeSpan;

		Ref<ShaderBuffer> m_ParticleBuffer;

		Ref<Shader> m_ShaderSimulate;
		Ref<Shader> m_ShaderDraw;
	};
}
