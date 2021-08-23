#include "ParticleEmitterSI.h"

RWByteAddressBuffer draw_iab : register(u6);

[numthreads(1, 1, 1)]
void cs_main(uint3 DTid : SV_DispatchThreadID)
{
	// Create draw argument buffer (VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation):
    draw_iab.Store4(DRAW_IAB_OFFSET_DRAWPARTICLES, uint4(6*THREADCOUNT_SIMULATION, 1, 0, 0));
}
