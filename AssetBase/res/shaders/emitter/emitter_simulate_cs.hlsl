#include "FrameCB.h"
#include "ParticleEmitterSI.h"

RWStructuredBuffer<Particle> particle_buffer : register(u0);

[numthreads(THREADCOUNT_SIMULATION, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
	uint particle_index = thread_id.x;
	Particle particle = particle_buffer[particle_index];


	if (particle.life > 0)
	{
		// particle.velocity += particle.force * u_DeltaTime;
		particle.position += particle.velocity * u_DeltaTime;
		particle.life -= u_DeltaTime;
	}
	else
	{
		particle.position = float3(0, 0, 0);
		particle.life = particle.max_life;
	}

	particle_buffer[particle_index] = particle;
}
