#include "CameraCB.h"
#include "TransformCB.h"

struct VERTEX_IN
{
	float3 position : POSITION;
};

struct PS_IN
{
	float4 position : SV_POSITION;
};

PS_IN vs_main(VERTEX_IN input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, mul(u_Transform, float4(input.position, 1.0)));

	return output;
}
