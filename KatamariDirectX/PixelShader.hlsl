cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    
    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    
    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
    
    appliedLight += diffuseLight;
    
    float3 finalColor = sampleColor * appliedLight;
    return float4(finalColor, 1.0f);
}

//Blending
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
//    return float4(pixelColor, alpha);
//}