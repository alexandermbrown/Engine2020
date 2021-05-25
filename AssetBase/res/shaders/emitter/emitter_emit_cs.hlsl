#include "FrameCB.h"
#include "ParticleEmitterSI.h"

RWStructuredBuffer<Particle> particle_buffer  : register(u0);
RWStructuredBuffer<uint> alive_buffer_current : register(u1);
RWStructuredBuffer<uint> alive_buffer_new     : register(u2);
RWStructuredBuffer<uint> dead_buffer          : register(u3);
RWByteAddressBuffer counter_buffer            : register(u4);

// From WickedEngine.
// returns a random float in range (0, 1). seed must be >0!
inline float rand(inout float seed, in float2 uv)
{
	float result = frac(sin(seed * dot(uv, float2(12.9898, 78.233))) * 43758.5453);
	seed += 1;
	return result;
}

[numthreads(THREADCOUNT_EMIT, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
	uint emit_count = counter_buffer.Load(PARTICLECOUNTER_OFFSET_REALEMITCOUNT);

	if (thread_id.x < emit_count)
	{
		// Emit particle.

		float2 uv = float2(u_RunTime + u_EmitterRandomness, (float)thread_id.x / (float)THREADCOUNT_EMIT);
		float seed = 0.23456;

		float3 pos = mul(u_EmitterTransform, float4(0, 0, 0, 1)).xyz;

		Particle particle;
		particle.position = pos;
		particle.velocity = float3(rand(seed, uv), rand(seed, uv), rand(seed, uv)) - 0.5f;
		particle.max_life = u_ParticleLifeSpan.x + (u_ParticleLifeSpan.y - u_ParticleLifeSpan.x) * rand(seed, uv);
		particle.life = particle.max_life;

		// New particle index retrieved from dead list (pop):
		uint dead_count;
		counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_DEADCOUNT, -1, dead_count);
		uint new_particle_index = dead_buffer[dead_count - 1];

		// Write out the new particle:
		particle_buffer[new_particle_index] = particle;

		// and add index to the alive list (push):
		uint alive_count;
		counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_ALIVECOUNT, 1, alive_count);
		alive_buffer_current[alive_count] = new_particle_index;
	}
}
