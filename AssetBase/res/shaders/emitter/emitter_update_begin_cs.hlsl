#include "ParticleEmitterSI.h"

RWByteAddressBuffer compute_iab : register(u5);

[numthreads(1, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
	// Fill dispatch argument buffer for emitting (ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ):
	//indirect_buffers.Store3(COMPUTE_IAB_OFFSET_DISPATCHEMIT, THREADCOUNT_EMIT, 1, 1));

	// Fill dispatch argument buffer for simulation (ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ):
	compute_iab.Store3(COMPUTE_IAB_OFFSET_DISPATCHSIMULATION, uint3(THREADCOUNT_SIMULATION, 1, 1));
}
