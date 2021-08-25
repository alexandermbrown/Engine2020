#include "ParticleEmitterSI.h"

RWByteAddressBuffer counter_buffer : register(u4);
RWByteAddressBuffer draw_iab : register(u6);

[numthreads(1, 1, 1)]
void cs_main(uint3 DTid : SV_DispatchThreadID)
{
    uint particle_count = counter_buffer.Load(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION);
	// Create draw argument buffer (VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation):
    draw_iab.Store4(DRAW_IAB_OFFSET_DRAWPARTICLES, uint4(6*particle_count, 1, 0, 0));
}
