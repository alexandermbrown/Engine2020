#include "ViewProjCB.h"
#include "ParticleEmitterSI.h"

static const int INDICES[] = {
	0, 1, 2, 0, 2, 3
};

static const float3 BILLBOARD[] = {
	float3(-0.5, -0.5, 0),	// 0
	float3(0.5, -0.5, 0),	// 1
	float3(0.5, 0.5, 0),	// 2
	float3(-0.5, 0.5, 0),	// 3
};

StructuredBuffer<Particle> particle_buffer : register(t1);

struct PS_IN
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PS_IN vs_main(uint vertex_id : SV_VERTEXID)
{
	PS_IN output;

	Particle particle = particle_buffer[vertex_id / 6];
	float3 quad_pos = BILLBOARD[INDICES[vertex_id % 6]];

	output.position = float4(particle.position, 1);
	output.position.xyz += quad_pos.xyz * 0.2;
	output.position = mul(u_ViewProj, output.position);
	output.color = particle.color;

	return output;
}
