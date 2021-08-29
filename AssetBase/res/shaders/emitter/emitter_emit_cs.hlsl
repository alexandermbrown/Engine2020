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

inline float rand_in_range(inout float seed, in float2 uv, float minimum, float maximum)
{
	return rand(seed, uv) * (maximum - minimum) + minimum;
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
		float3 half_volume = u_EmitVolume / 2.0;
		float3 emit_offset = float3(
			rand_in_range(seed, uv, -half_volume.x, half_volume.x),
			rand_in_range(seed, uv, -half_volume.y, half_volume.y),
			rand_in_range(seed, uv, -half_volume.z, half_volume.z)
		);

		if (u_RelativeToWorld) {
			particle.position = mul(u_EmitterTransform, float4(emit_offset, 1.0)).xyz;
		} else {
			particle.position = emit_offset;
		}

		particle.angle = rand_in_range(seed, uv, u_Rotation.x, u_Rotation.y);
		particle.scale = u_Scale * u_ScaleGraph[0].y;

		float speed = rand_in_range(seed, uv, u_SpeedRange.x, u_SpeedRange.y);
		float angle = rand(seed, uv) * 2.0 * PI;
		particle.velocity = float3(speed * cos(angle), speed * sin(angle), 0.0);
		particle.angular_velocity = rand_in_range(seed, uv, u_Rotation.z, u_Rotation.w);

        particle.start_life = rand_in_range(seed, uv, u_LifeSpan.x, u_LifeSpan.y);
        particle.life_left = particle.start_life;
		particle.color = float4(
			rand(seed, uv) * 0.5 + 0.4,
			rand(seed, uv) * 0.5 + 0.4,
			rand(seed, uv) * 0.5 + 0.4,
			u_AlphaGraph[0].y
		);

		particle._pad0 = 0;
		particle._pad1 = 0;
		particle._pad2 = 0;

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
