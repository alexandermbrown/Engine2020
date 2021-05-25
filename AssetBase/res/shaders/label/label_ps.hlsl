#include "FontCB.h"

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	int texindex : TEXINDEX;
};

// Texture2D u_Texture : register(t0);
// SamplerState u_Sampler : register(s0);
Texture2D u_Texture0 : register(t0);
Texture2D u_Texture1 : register(t1);
Texture2D u_Texture2 : register(t2);
Texture2D u_Texture3 : register(t3);
Texture2D u_Texture4 : register(t4);
Texture2D u_Texture5 : register(t5);
Texture2D u_Texture6 : register(t6);
Texture2D u_Texture7 : register(t7);
SamplerState u_Sampler0 : register(s0);
SamplerState u_Sampler1 : register(s1);
SamplerState u_Sampler2 : register(s2);
SamplerState u_Sampler3 : register(s3);
SamplerState u_Sampler4 : register(s4);
SamplerState u_Sampler5 : register(s5);
SamplerState u_Sampler6 : register(s6);
SamplerState u_Sampler7 : register(s7);

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	float4 tex_color;
	[forcecase] switch(input.texindex)
	{
		case 0: tex_color = u_Texture0.Sample(u_Sampler0, input.texcoord); break;
		case 1: tex_color = u_Texture1.Sample(u_Sampler1, input.texcoord); break;
		case 2: tex_color = u_Texture2.Sample(u_Sampler2, input.texcoord); break;
		case 3: tex_color = u_Texture3.Sample(u_Sampler3, input.texcoord); break;
		case 4: tex_color = u_Texture4.Sample(u_Sampler4, input.texcoord); break;
		case 5: tex_color = u_Texture5.Sample(u_Sampler5, input.texcoord); break;
		case 6: tex_color = u_Texture6.Sample(u_Sampler6, input.texcoord); break;
		case 7: tex_color = u_Texture7.Sample(u_Sampler7, input.texcoord); break;
	}
	float3 color_sample = tex_color.rgb;

	float sig_dist = u_DistanceFactor * (median(color_sample.r, color_sample.g, color_sample.b) - 0.5);
	float opacity = clamp(sig_dist + 0.5, 0.0, 1.0);
	return float4(u_Color.rgb, opacity * u_Color.a);
}
