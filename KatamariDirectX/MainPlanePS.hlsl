struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
	float3 inColor : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(input.inColor, 1.0f);
}
