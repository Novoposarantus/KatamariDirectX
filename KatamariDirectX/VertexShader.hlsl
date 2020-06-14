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
    float4 camPos;
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
    float4 outShadowCoord : SHADDOW_TEXCOORD;
    float3 viewDirection : TEXCOORD1;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4x4 camVPMatrix = mul(camViewMatrix, camProjMatrix);
    float4x4 camShadowVPMatrix = mul(camShadowViewMatrix, camShadowProjMatrix);
    
    float4x4 mwshWVPMatrix = mul(mul(meshTransformMatrix, worldMatrix), camVPMatrix);
    float4x4 meshWorldMatrix = mul(meshTransformMatrix, worldMatrix);
    float4x4 meshViewProjShadow = mul(mul(meshTransformMatrix, worldMatrix), camShadowVPMatrix);
    
	
    output.outPositionWVP = mul(float4(input.inPos, 1.0f), mwshWVPMatrix);
    output.outPosition = input.inPos;
    output.outTexCoord = input.inTexCoord;
    output.outSpecColor = input.inSpecColor;
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), meshWorldMatrix));
    output.outShadowCoord = mul(float4(input.inPos, 1.0f), meshViewProjShadow);
    
    float4 worldPosition = mul(float4(input.inPos, 1.0f), worldMatrix);
    output.viewDirection = camPos.xyz - worldPosition.xyz;
    output.viewDirection = normalize(output.viewDirection);
    
    return output;
}
