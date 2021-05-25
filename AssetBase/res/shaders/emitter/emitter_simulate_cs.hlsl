#include "FrameCB.h"
#include "ParticleEmitterSI.h"

RWStructuredBuffer<Particle> particle_buffer  : register(u0);
RWStructuredBuffer<uint> alive_buffer_current : register(u1);
RWStructuredBuffer<uint> alive_buffer_new     : register(u2);
RWStructuredBuffer<uint> dead_buffer          : register(u3);
RWByteAddressBuffer counter_buffer            : register(u4);

[numthreads(THREADCOUNT_SIMULATION, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
	uint alive_count = counter_buffer.Load(PARTICLECOUNTER_OFFSET_ALIVECOUNT);

	if (thread_id.x < alive_count)
	{
		const float dt = u_DeltaTime;

		uint particle_index = alive_buffer_current[thread_id.x];
		Particle particle = particle_buffer[particle_index];

		if (particle.life > 0)
		{
			// Simulate particle.
			// particle.velocity += particle.force * dt;
			particle.position += particle.velocity * dt;
			particle.life -= dt;

			particle_buffer[particle_index] = particle;

			// Add to new alive list:
			uint new_alive_index;
			counter_buffer.InterlockedAdd(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION, 1, new_alive_index);
			alive_buffer_new[new_alive_index] = particle_index;
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
