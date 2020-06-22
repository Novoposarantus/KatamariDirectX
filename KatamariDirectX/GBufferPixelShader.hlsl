struct PS_INPUT
{
    float4 inPosWVP : SV_POSITION;
    float4 inPositionW : POSITION1;
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
    float3 inSpecColor : SPEC_COLOR;
    float4 inShadowCoord : SHADDOW_TEXCOORD;
    float3 viewDirection : TEXCOORD1;
};

struct PS_OUT
{
    float4 normalColor;
    float4 depthColor;
    float4 ambientColor;
    float4 textureColor;
};

Texture2D objTexture : register(t0);

SamplerState wrapSamplerState : register(s0);
SamplerState clampSamplerState : register(s1);

float3 GetAmbient()
{
    return float3(0.2f, 0.2f, 0.2f);
}

PS_OUT main(PS_INPUT input) : SV_Target
{
    float3 textureColor = objTexture.Sample(wrapSamplerState, input.inTexCoord);
    float depthValue = input.inPosWVP.z / input.inPosWVP.w;
    
    PS_OUT result;
    result.normalColor = float4(input.inNormal, 1);
    result.depthColor = float4(depthValue, depthValue, depthValue, 1.0f);
    result.ambientColor = input.inPositionW;
    result.textureColor = float4(textureColor, 1);
    
    return result;
}