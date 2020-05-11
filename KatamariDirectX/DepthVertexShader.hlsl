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
    float3 inSpecColor : SPEC_COLOR;
};

struct VS_OUTPUT
{
    float4 positionWVP : SV_POSITION;
    float4 position : POSITION;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4x4 camVPMatrix = mul(camShadowViewMatrix, camShadowProjMatrix);
    
    float4x4 mwshWVPMatrix = mul(mul(meshTransformMatrix, worldMatrix), camVPMatrix);
	
    output.positionWVP = mul(float4(input.inPos, 1.0f), mwshWVPMatrix);
    output.position = output.positionWVP;
    return output;
}
