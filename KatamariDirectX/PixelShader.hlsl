cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float3 dynamicLightPosition;
    float dynamicLightStrength;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    //float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 sampleColor = input.inNormal;
    
    
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    
    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    
    float3 diffuseLightIntensity = dot(vectorToLight, input.inNormal);
    
    float3 finalColor = sampleColor * appliedLight;
    return float4(finalColor, 1.0f);
}

//Blending
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
//    return float4(pixelColor, alpha);
//}