#include "CameraCB.h"
#include "TransformCB.h"

struct VERTEX_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float4 world_pos : POSITION;
	float3 normal : NORMAL;
};

PS_IN vs_main(VERTEX_IN input)
{
	PS_IN output;

	output.world_pos = mul(u_Transform, float4(input.position, 1.0));
	output.position = mul(u_ViewProj, output.world_pos);
	// Apply scaling and rotation to normals.
	output.normal = mul((float3x3)u_Transform, input.normal);

	return output;
}
