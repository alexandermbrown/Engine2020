#include "CameraCB.h"
#include "TransformCB.h"

struct VS_IN
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float texindex : TEXINDEX;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	int texindex : TEXINDEX;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, mul(u_Transform, float4(input.position, 1.0)));
	output.texcoord = input.texcoord;
	output.texindex = int(input.texindex);

	return output;
}
