#pragma once
#ifndef LI_SI_PARTICLE_EMITTER_H
#define LI_SI_PARTICLE_EMITTER_H

#include "ShaderInterop.h"

#define LI_GRAPH_NODE_COUNT_MAX 8

#ifdef __cplusplus
namespace Li {
#endif
	// Note: Variable order matters. Floats ensure 16 byte alignment.
	struct Particle
	{
		float4 color;
		float3 position;
		float angle;
		float3 scale;
		float life_left;
		float3 velocity;
		float start_life;
		float angular_velocity;
		float _pad0;
		float _pad1;
		float _pad2;
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
		aligned_float3 u_Scale;
		uint u_RelativeToWorld;
		// Initial angle (min, max), rotational velocity (min, max).
		aligned_float4 u_Rotation;
		aligned_float3 u_EmitVolume;
		float u__EmitterPad0;
		aligned_float3 u_Acceleration;
		float u__EmitterPad1;
		//float pad;
		// Note: We only use the first 2 elements of each float4.
		// The remaining floats are padding required by D3D11.
		// https://geidav.wordpress.com/2013/03/05/hidden-hlsl-performance-hit-accessing-unpadded-arrays-in-constant-buffers/
		float4 u_ScaleGraph[LI_GRAPH_NODE_COUNT_MAX];
		float4 u_RedGraph[LI_GRAPH_NODE_COUNT_MAX];
		float4 u_BlueGraph[LI_GRAPH_NODE_COUNT_MAX];
		float4 u_GreenGraph[LI_GRAPH_NODE_COUNT_MAX];
		float4 u_AlphaGraph[LI_GRAPH_NODE_COUNT_MAX];
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
