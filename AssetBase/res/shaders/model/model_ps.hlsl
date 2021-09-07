#include "CameraCB.h"

struct PS_IN
{
	float4 position : SV_POSITION;
	float4 world_pos : POSITION;
	float3 normal : NORMAL;
};

float4 ps_main(PS_IN input) : SV_TARGET
{
	float3 frag_to_cam = normalize((float3)u_CameraPos - input.world_pos);
	float3 normal = normalize(input.normal);

	float facing_frac = dot(frag_to_cam, normal);
	// float outline = facing_frac;
	float outline = 1.0 - pow(1.0 - facing_frac, 2);

	// float outline;
	// if (facing_frac < 0.2)
	// 	outline = 0.0;
	// else
	// 	outline = 1.0;

	return float4(outline, outline, outline, 1.0);
}
