
struct PS_IN
{
	float4 position : SV_POSITION;
};

float4 ps_main(PS_IN input) : SV_TARGET
{
	return float4(0.8, 1.0, 1.0, 1.0);
}
