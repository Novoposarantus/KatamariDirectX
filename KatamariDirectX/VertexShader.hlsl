cbuffer cBuffer : register(b0)
{
    row_major float4x4 wvpMatrix;
    row_major float4x4 worldMatrix;
    row_major float4x4 wvpLight;
    
    float3 lightPosition;
    float padding;
};

struct VS_INPUT
{
	float4 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
    float4 inWorldViewPos : WORLD_VIEW_POSITION;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POSITION;
    float4 outWorldViewPos : WORLD_VIEW_POSITION;
};


VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
    output.outPosition = mul(input.inPos, wvpMatrix);
    output.outWorldViewPos = mul(input.inPos, wvpLight);
	output.outTexCoord = input.inTexCoord;
    output.outNormal = normalize(mul(input.inNormal, (float3x3)worldMatrix));
    float4 worldPosition = mul(input.inPos, worldMatrix);
    output.outWorldPos = normalize(lightPosition.xyz - worldPosition.xyz);
	return output;
}