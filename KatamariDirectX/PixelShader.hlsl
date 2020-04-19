cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float dynamicLightStrength;
    float3 dynamicLightPosition;
    
    float dynamicLightAttenuation_a;
    float dynamicLightAttenuation_b;
    float dynamicLightAttenuation_c;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
    float4 inWorldViewPos : WORLD_VIEW_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
Texture2D depthMapTexture : TEXTURE : register(t1);

SamplerState objSamplerState : SAMPLER : register(s0);
SamplerState depthSampleState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float3 textureColor;
    
	// ��������� �������� �������� ������������� ��� ���������� ������� �������� � ��������� �������
    float bias = 0.001f;

    float3 color = ambientLightColor * ambientLightStrength;

	// ���������� ��������� ������������� ��������
    projectTexCoord.x = input.inWorldViewPos.x / input.inWorldViewPos.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.inWorldViewPos.y / input.inWorldViewPos.w / 2.0f + 0.5f;

	// ��������� �� ��������������� ���������� � �������� 0 � 1. ���� ��, �� ������� ��������� � ��������� �����
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        depthValue = depthMapTexture.Sample(depthSampleState, projectTexCoord).r;

		// ���������� ������� �����
        lightDepthValue = input.inWorldViewPos.z / input.inWorldViewPos.w;

		// ��������� �������� �� lightDepthValue
        lightDepthValue = lightDepthValue - bias;

		// ��������� ������� ������� ����� � ������� �����, ��� ����������� ����, ������� ��� ������� �������
		// ���� ���� ����� ��������, �� ������� �������; ���� ���, �� ������� ������� � ������ ������� ���� �� ���
        if (lightDepthValue < depthValue)
        {
            float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    
            float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);
    
            float3 distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
    
            float3 attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));
    
            diffuseLightIntensity *= attenuationFactor;
		    // ���������� ���������� ����� � �������
            lightIntensity = saturate(dot(input.inNormal, input.inWorldPos));

            if (lightIntensity > 0.0f)
            {
				// ����������� ��������������� ���������� (diffuse) ����� �� ������ ���������� ����� � ������������� �����
                color += (dynamicLightColor * dynamicLightStrength * dynamicLightColor * lightIntensity);
                color = saturate(color);
            }
        }
    }

    textureColor = objTexture.Sample(objSamplerState, input.inTexCoord);

    return float4(color * textureColor, 1.0f);
}