#include "FrameCB.h"
#include "ParticleEmitterSI.h"

RWStructuredBuffer<Particle> particle_buffer : register(u0);
ConsumeStructuredBuffer<uint> dead_list : register(u1);

[numthreads(THREADCOUNT_SIMULATION, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
    uint index = dead_list.Consume();

    
}
