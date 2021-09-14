#include "CameraCB.h"

struct PS_IN
{
	float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 ps_main(PS_IN input) : SV_TARGET
{
	return float4(input.texcoord, 0.0, 1.0);
}
