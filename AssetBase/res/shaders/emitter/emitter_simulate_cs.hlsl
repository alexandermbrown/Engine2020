#include "FrameCB.h"
#include "ParticleEmitterSI.h"

RWStructuredBuffer<Particle> particles        : register(u0);
RWStructuredBuffer<uint> alive_buffer_current : register(u1);
RWStructuredBuffer<uint> alive_buffer_new     : register(u2);
RWStructuredBuffer<uint> dead_buffer          : register(u3);
RWByteAddressBuffer counter_buffer            : register(u4);
RWStructuredBuffer<float> distance_buffer     : register(u6);

float graph_lerp(float4 val_vs_life[LI_GRAPH_NODE_COUNT_MAX], float life)
{
	// each elem of val_vs_life is (life_fraction, value, nothing, nothing).
	// val_vs_life[0].x is assumed to always be 0.
	for (int i = 1; i < LI_GRAPH_NODE_COUNT_MAX; i++)
	{
		if (life <= val_vs_life[i].x)
		{
			return lerp(
				val_vs_life[i - 1].y,
				val_vs_life[i].y,
				(life - val_vs_life[i - 1].x) / (val_vs_life[i].x - val_vs_life[i - 1].x)
			);
		}
	}
	return val_vs_life[LI_GRAPH_NODE_COUNT_MAX - 1].y;
}

[numthreads(THREADCOUNT_SIMULATION, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
	uint alive_count = counter_buffer.Load(PARTICLECOUNTER_OFFSET_ALIVECOUNT);

	if (thread_id.x < alive_count)
	{
		uint particle_index = alive_buffer_current[thread_id.x];
		Particle particle = particles[particle_index];

		if (particle.life_left > 0)
		{
			particle.velocity += u_Acceleration * u_DeltaTime;
			particle.position += particle.velocity * u_DeltaTime;
			particle.angle += particle.angular_velocity * u_DeltaTime;
			particle.life_left -= u_DeltaTime;

			float life_fraction = clamp(1.0 - (particle.life_left / particle.start_life), 0.0, 1.0);
			particle.scale = u_Scale * graph_lerp(u_ScaleGraph, life_fraction);
			particle.color.r = graph_lerp(u_RedGraph,   life_fraction);
			particle.color.g = graph_lerp(u_GreenGraph, life_fraction);
			particle.color.b = graph_lerp(u_BlueGraph,  life_fraction);
			particle.color.a = graph_lerp(u_AlphaGraph, life_fraction);

			particles[particle_index] = particle;

			// Add to new alive list:
			uint new_alive_index;
			counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION, 1, new_alive_index);
			alive_buffer_new[new_alive_index] = particle_index;

			// TODO: Check if it is reversed or not.
			distance_buffer[particle_index] = particle.position.z;
		}
		else
		{
			// Kill particle.
			uint dead_index;
			counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_DEADCOUNT, 1, dead_index);
			dead_buffer[dead_index] = particle_index;
		}
	}
}
