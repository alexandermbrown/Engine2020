#include "ParticleEmitterSI.h"

RWByteAddressBuffer counter_buffer : register(u4);
RWByteAddressBuffer compute_iab : register(u5);

[numthreads(1, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{
	// Load dead particle count:
	uint dead_count = counter_buffer.Load(PARTICLECOUNTER_OFFSET_DEADCOUNT);

	// Load alive particle count:
	uint alive_count_new = counter_buffer.Load(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION);

	// We can not emit more than there are free slots in the dead list:
	uint real_emit_count = min(dead_count, u_EmitCount);

	// Fill dispatch argument buffer for emitting (ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ):
	compute_iab.Store3(COMPUTE_IAB_OFFSET_DISPATCHEMIT,
		uint3(ceil((float)real_emit_count / (float)THREADCOUNT_EMIT), 1, 1));

	// Fill dispatch argument buffer for simulation (ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ):
	compute_iab.Store3(COMPUTE_IAB_OFFSET_DISPATCHSIMULATION,
		uint3(ceil((float)(alive_count_new + real_emit_count) / (float)THREADCOUNT_SIMULATION), 1, 1));

	// Copy new alivelistcount to current alivelistcount:
	counter_buffer.Store(PARTICLECOUNTER_OFFSET_ALIVECOUNT, alive_count_new);

	// Reset new alivecount:
	counter_buffer.Store(PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION, 0);

	// Write real emit count:
	counter_buffer.Store(PARTICLECOUNTER_OFFSET_REALEMITCOUNT, real_emit_count);
}
