
struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

PS_IN vs_main(uint id : SV_VERTEXID)
{
    // Full screen triangle.
    // Source: https://www.slideshare.net/DevCentralAMD/vertex-shader-tricks-bill-bilodeau
	PS_IN output;

	output.position.x = (float)(id / 2) * 4.0 - 1.0;
	output.position.y = (float)(id % 2) * 4.0 - 1.0;
    output.position.z = 0.0;
    output.position.w = 1.0;

    output.texcoord.x = (float)(id / 2) * 2.0;
    output.texcoord.y = 1.0 - (float)(id % 2) * 2.0;

	return output;
}
