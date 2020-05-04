cbuffer MatrixBuffer
{
    row_major float4x4 WVP;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
	
    output.position = mul(float4(input.inPos, 1.0f), WVP);
	// Пишем позицию в depthPosition
    output.depthPosition = output.position;
	
    return output;
}