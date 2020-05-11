cbuffer cBuffer : register(b0)
{
    row_major float4x4 worldMatrix;
    row_major float4x4 meshTransformMatrix;
};

cbuffer cBuffer2 : register(b1)
{
    row_major float4x4 camShadowViewMatrix;
    row_major float4x4 camShadowProjMatrix;
    
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
    
    float4x4 shadowWVP = meshTransformMatrix * worldMatrix * camShadowViewMatrix * camShadowProjMatrix;
	
    output.position = mul(float4(input.inPos, 1.0f), shadowWVP);
	// Пишем позицию в depthPosition
    output.depthPosition = output.position;
	
    return output;
}