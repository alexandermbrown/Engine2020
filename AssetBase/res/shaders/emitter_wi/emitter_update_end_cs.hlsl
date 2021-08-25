#include "ParticleEmitterSI_WI.h"

ByteAddressBuffer counter_buffer : register(t4);
RWByteAddressBuffer draw_iab : register(u6);

[numthreads(1, 1, 1)]
void cs_main(uint3 DTid : SV_DispatchThreadID)
{
	uint particle_count = counter_buffer.Load(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION);
	// Create draw argument buffer (VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation):
	draw_iab.Store4(DRAW_IAB_OFFSET_DRAWPARTICLES, uint4(4, particle_count, 0, 0));
}
