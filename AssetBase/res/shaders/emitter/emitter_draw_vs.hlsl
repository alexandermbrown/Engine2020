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
StructuredBuffer<uint> alive_buffer : register(t2);

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

PS_IN vs_main(uint vertex_id : SV_VERTEXID)
{
	PS_IN output;

	Particle particle = particle_buffer[alive_buffer[vertex_id / 6]];
	float3 quad_pos = BILLBOARD[INDICES[vertex_id % 6]];
	float3 tex_coord = quad_pos + float3(0.5, 0.5, 0.0);
	quad_pos *= particle.scale;

	float cos_rot = cos(particle.angle);
	float sin_rot = sin(particle.angle);
	quad_pos.xy = float2(
		quad_pos.x * cos_rot - quad_pos.y * sin_rot,
		quad_pos.x * sin_rot + quad_pos.y * cos_rot
	);

	output.position = float4(particle.position, 1);
	output.position.xyz += quad_pos.xyz;

	if (!u_RelativeToWorld) {
		output.position = mul(u_EmitterTransform, output.position);
	}
	output.position = mul(u_ViewProj, output.position);
	output.texcoord = tex_coord;
	output.color = particle.color;

	return output;
}
