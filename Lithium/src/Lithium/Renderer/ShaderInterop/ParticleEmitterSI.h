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
		float max_life;
		float3 velocity;
		float life;
	};

	LI_CBUFFER(EmitterCB, 4)
	{
		aligned_float4x4 u_EmitterTransform;
		aligned_uint u_EmitCount;
		aligned_float u_EmitterRandomness;
		aligned_float2 u_ParticleLifeSpan;
	};

	static const uint THREADCOUNT_SIMULATION = 256;

	static const uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION = 0;
	static const uint DRAW_IAB_OFFSET_DRAWPARTICLES = 0;


#ifdef __cplusplus
}
#endif

#endif // LI_SI_PARTICLE_EMITTER_H
