#pragma once
#ifndef LI_SI_PARTICLE_EMITTER_H
#define LI_SI_PARTICLE_EMITTER_H

#include "ShaderInterop.h"

#ifdef __cplusplus
namespace Li {
#endif
	// Note: Variable order matters. Floats ensure 16 byte alignment.
	struct Particle
	{
		float4 color;
		float3 position;
		float life;
		float3 velocity;
		float _pad;
	};

	// Note: Order matters.
	struct ParticleCounters
	{
		uint alive_count;
		uint dead_count;
		uint real_emit_count;
		uint alive_count_after_sim;
	};

	// Note: Order matters.
	LI_CBUFFER(EmitterCB, 4)
	{
		aligned_float4x4 u_EmitterTransform;
		aligned_float2 u_LifeSpan;
		uint u_EmitCount;
		float u_EmitterRandomness;
		aligned_float2 u_SpeedRange;
	};

	static const uint THREADCOUNT_EMIT = 256;
	static const uint THREADCOUNT_SIMULATION = 256;

	static const uint PARTICLECOUNTER_OFFSET_ALIVECOUNT = 0;
	static const uint PARTICLECOUNTER_OFFSET_DEADCOUNT = PARTICLECOUNTER_OFFSET_ALIVECOUNT + 4;
	static const uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT = PARTICLECOUNTER_OFFSET_DEADCOUNT + 4;
	static const uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION = PARTICLECOUNTER_OFFSET_REALEMITCOUNT + 4;

	static const uint COMPUTE_IAB_OFFSET_DISPATCHEMIT = 0;
	static const uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION = COMPUTE_IAB_OFFSET_DISPATCHEMIT + (3 * 4);

	static const uint DRAW_IAB_OFFSET_DRAWPARTICLES = 0;


#ifdef __cplusplus
}
#endif

#endif // LI_SI_PARTICLE_EMITTER_H
