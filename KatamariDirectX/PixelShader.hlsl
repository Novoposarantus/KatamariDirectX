cbuffer lightBuffer : register(b0)
{
    float3 directionalLightColor;
    float directionalLightStrenght;
    float3 directionalLightDir;
    float dirPadding;
    float3 camPos;
    float posPadding;
    float3 specPowerPadding;
    float specPower;
}

struct PS_INPUT
{
    float4 inPosWVP : SV_POSITION;
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
    float3 inSpecColor : SPEC_COLOR;
    float4 inShadowCoord : SHADDOW_TEXCOORD;
    float3 viewDirection : TEXCOORD1;
};

Texture2D objTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D curLumiTexture : register(t2);
Texture2D prevLumiTexture : register(t3);

SamplerState wrapSamplerState : register(s0);
SamplerState clampSamplerState : register(s1);

float3 GetAmbient()
{
    return float3(0.2f, 0.2f, 0.2f);
}

float GetShadow(float4 shadowCoord)
{
    float2 projectTexCoord;
    projectTexCoord.x = shadowCoord.x / shadowCoord.w / 2.0f + 0.5f;
    projectTexCoord.y = -shadowCoord.y / shadowCoord.w / 2.0f + 0.5f;
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        const float shadowMapTexelSize = 1.0f / 1000.f;
        const float dx = shadowMapTexelSize;
        const float shadowCoordV = shadowCoord.z / shadowCoord.w;
        float bias = 0.001f;
    
        if (shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord).r)
        {
            return 1.0f;
        }
        
        //float depth = depthMapTexture.SampleCmpLevelZero(depthobjSamplerState, float2(projectTexCoord.x, 1.0f - projectTexCoord.y), shadowCoordV).x;
        
        bool d0 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord).r;
        float d1 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(dx, 0)).r;
        float d2 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(0, dx)).r;
        float d3 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(-dx, 0)).r;
        float d4 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(0, -dx)).r;
        
        float d5 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(dx, dx)).r;
        float d6 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(dx, -dx)).r;
        float d7 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(-dx, dx)).r;
        float d8 = shadowCoordV - bias < depthMapTexture.Sample(clampSamplerState, projectTexCoord + float2(-dx, -dx)).r;
        
        float res = (d0 * 2 + d1 + d2 + d3 + d4 + d5 + d6 + d7 + d8) / 10.0f;
        res *= 1 - dot(min(pow(abs(projectTexCoord - 0.5f) * 2, 2), 1.0f), float2(1, 1));
        return res;
        
        return d0;
    }
    
    return 1;
}

float4 main(PS_INPUT input) : SV_Target
{
    float3 textureColor = objTexture.Sample(wrapSamplerState, input.inTexCoord);
    
    float3 lightV = -directionalLightDir;
    
    float3 lowLambert = max(dot(lightV, input.inNormal), 0);
    
    float3 lightStrenght = directionalLightColor * directionalLightStrenght * lowLambert;
    
    float3 color = lightStrenght * GetShadow(input.inShadowCoord) + GetAmbient();
    float3 finalColor = color * textureColor;
    
    
    float lum = curLumiTexture.Load(int3(0, 0, 10));
    float prevlum = prevLumiTexture.Load(int3(0, 0, 10));
    
    float3 ldr;

    if (lum > 0)
    {
	
        float l = lerp(prevlum, lum, 0.25);
        ldr = float3(1.0f, 1.0f, 1.0f) - exp(-textureColor * 0.1 / l);
    }
    else
    {
        ldr = float3(1.0f, 1.0f, 1.0f) - exp(-textureColor * 2);
    }
    
    float3 reflection = normalize(2 * directionalLightStrenght * input.inNormal + directionalLightDir);
    float3 specular = pow(saturate(dot(reflection, input.viewDirection)), specPower) + input.inSpecColor;
    finalColor = saturate(finalColor + specular);
    //return float4(ldr, 1);
    return float4(finalColor, 1);
}