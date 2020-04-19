#pragma once
#include <DirectXMath.h>


struct CB_VS_VertexShader
{
	DirectX::SimpleMath::Matrix wvpMatrix;
	DirectX::SimpleMath::Matrix worldMatrix;
	DirectX::SimpleMath::Matrix wvpLight;

	DirectX::SimpleMath::Vector3 lightPosition;
	float padding;
};


struct CB_PS_Light
{
	DirectX::SimpleMath::Vector3 ambientLightColor;
	float ambientLightStrength;

	DirectX::SimpleMath::Vector3 dynamicLightColor;
	float dynamicLightStrength;
	DirectX::SimpleMath::Vector3 dynamicLightPosition;
	float dynamicLightAttenuation_a;
	float dynamicLightAttenuation_b;
	float dynamicLightAttenuation_c;
};

struct CB_VS_DEPTH
{
	DirectX::SimpleMath::Matrix WVP;
};
