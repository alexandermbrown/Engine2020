#include "CameraCB.h"

struct VS_IN
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, float4(input.position, 1.0));
	output.color = input.color;

	return output;
}
