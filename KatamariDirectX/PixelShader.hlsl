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
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

Texture2D objTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState objSamplerState : register(s0);
SamplerState depthobjSamplerState : register(s1);

float4 main(PS_INPUT input) : SV_Target
{
    
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float4 textureColor;

	// ��������� �������� �������� ������������� ��� ���������� ������� �������� � ��������� �������
    float bias = 0.001f;

    float4 color = float4(ambientLightColor, 1.0f) * ambientLightStrength;

	// ���������� ��������� ������������� ��������
    projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// ��������� �� ��������������� ���������� � �������� 0 � 1. ���� ��, �� ������� ��������� � ��������� �����
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        depthValue = depthMapTexture.Sample(objSamplerState, projectTexCoord).r;

		// ���������� ������� �����
        lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// ��������� �������� �� lightDepthValue
        lightDepthValue = lightDepthValue - bias;

		// ��������� ������� ������� ����� � ������� �����, ��� ����������� ����, ������� ��� ������� �������
		// ���� ���� ����� ��������, �� ������� �������; ���� ���, �� ������� ������� � ������ ������� ���� �� ���
        if (lightDepthValue < depthValue)
        {
		    // ���������� ���������� ����� � �������
            lightIntensity = saturate(dot(input.inNormal, input.lightPos));

            if (lightIntensity > 0.0f)
            {
				// ����������� ��������������� ���������� (diffuse) ����� �� ������ ���������� ����� � ������������� �����
                color += (float4(dynamicLightColor, 1.0f) * lightIntensity);
                color = saturate(color);
            }
        }
    }

    textureColor = objTexture.Sample(depthobjSamplerState, input.inTexCoord);

    return color * textureColor;
}