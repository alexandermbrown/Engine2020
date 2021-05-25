#include "ViewProjCB.h"
#include "ParticleEmitterSI.h"

static const float3 BILLBOARD[] = {
	float3(-1, -1, 0),	// 0
	float3(1, -1, 0),	// 1
	float3(-1, 1, 0),	// 2
	float3(1, 1, 0),	// 4
};

StructuredBuffer<Particle> particle_buffer : register(t1);
StructuredBuffer<uint> alive_list : register(t2);

struct PS_IN
{
	float4 position : SV_POSITION;
};

PS_IN vs_main(uint vertex_id : SV_VERTEXID, uint instance_id : SV_INSTANCEID)
{
	PS_IN output;

	Particle particle = particle_buffer[alive_list[instance_id]];

	float3 quad_pos = BILLBOARD[vertex_id];

	output.position = float4(particle.position, 1);
	output.position.xyz += quad_pos.xyz;
	output.position = mul(u_ViewProj, output.position);

	// output.position = float4(0.5, 0.5, 0.5, 1);

	return output;
}
