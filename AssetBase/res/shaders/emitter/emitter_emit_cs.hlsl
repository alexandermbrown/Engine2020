#include "Globals.hlsli"
#include "FrameCB.h"
#include "ParticleEmitterSI.h"

RWStructuredBuffer<Particle> particles        : register(u0);
RWStructuredBuffer<uint> alive_buffer_current : register(u1);
RWStructuredBuffer<uint> alive_buffer_new     : register(u2);
RWStructuredBuffer<uint> dead_buffer          : register(u3);
RWByteAddressBuffer counter_buffer            : register(u4);

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
		float2 uv = float2(u_RunTime + u_EmitterRandomness, (float)thread_id.x / (float)THREADCOUNT_EMIT);
		float seed = 0.38224;

		Particle particle;
        particle.position = float3(0, 0, 0);
		particle.rotation = 0.0;
		particle.scale = u_Scale * u_ScaleGraph[0].y;

		float speed = rand(seed, uv) * (u_SpeedRange.y - u_SpeedRange.x) + u_SpeedRange.x;
		float angle = rand(seed, uv) * 2.0 * PI;
		particle.velocity = float3(speed * cos(angle), speed * sin(angle), 0.0);

        particle.start_life = rand(seed, uv) * (u_LifeSpan.y - u_LifeSpan.x) + u_LifeSpan.x;
        particle.life_left = particle.start_life;
		particle.color = float4(
			rand(seed, uv) * 0.5 + 0.4,
			rand(seed, uv) * 0.5 + 0.4,
			rand(seed, uv) * 0.5 + 0.4,
			u_AlphaGraph[0].y
		);

        // New particle index retrieved from dead list (pop):
		uint dead_count;
		counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_DEADCOUNT, -1, dead_count);
		uint new_particle_index = dead_buffer[dead_count - 1];

		// Write out the new particle:
		particles[new_particle_index] = particle;

		// and add index to the alive list (push):
		uint alive_count;
		counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_ALIVECOUNT, 1, alive_count);
		alive_buffer_current[alive_count] = new_particle_index;
    }
}
