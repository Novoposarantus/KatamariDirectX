cbuffer cBuffer : register(b0)
{
    row_major float4x4 worldMatrix;
    row_major float4x4 meshTransformMatrix;
};

cbuffer cBuffer2 : register(b1)
{
    row_major float4x4 camViewMatrix;
    row_major float4x4 camProjMatrix;
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
    float4 outPositionWVP : SV_POSITION;
    float3 outPosition : POSITION;
    float2 outTexCoord : TEXCOORD0;
    float3 outNormal : NORMAL;
    float3 outSpecColor : SPEC_COLOR;
    float3 outShadowCoord : SHADDOW_TEXCOORD;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4x4 camVPMatrix = mul(camViewMatrix, camProjMatrix);
    float4x4 camShadowVPMatrix = mul(camShadowViewMatrix, camShadowProjMatrix);
    
    float4x4 mwshWVPMatrix = mul(mul(meshTransformMatrix, worldMatrix), camVPMatrix);
    float4x4 meshWorldMatrix = mul(meshTransformMatrix, worldMatrix);
    float4x4 meshViewProjShadow = mul(meshTransformMatrix, camShadowVPMatrix);
    
	
    output.outPositionWVP = mul(float4(input.inPos, 1.0f), mwshWVPMatrix);
    output.outPosition = input.inPos;
    output.outTexCoord = input.inTexCoord;
    output.outSpecColor = input.inSpecColor;
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), meshWorldMatrix));
    output.outShadowCoord = mul(meshViewProjShadow, float4(input.inPos, 1.0f)).xyz;
    output.outShadowCoord.xy = float2(output.outShadowCoord.x * 0.5 + 0.5f, -output.outShadowCoord.y * 0.5 + 0.5f);
    return output;
}
