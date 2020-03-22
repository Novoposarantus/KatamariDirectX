#pragma once
#include <DirectXMath.h>


struct CB_VS_VertexShader
{
	DirectX::SimpleMath::Matrix wvpMatrix;
	DirectX::SimpleMath::Matrix worldMatrix;
};

struct CB_PS_Alpha
{
	float alpha = 1.0f;
};

struct CB_PS_Light
{
	DirectX::SimpleMath::Vector3 ambientLightColor;
	float ambientLightStrength;

	DirectX::SimpleMath::Vector3 dynamicLightColor;
	DirectX::SimpleMath::Vector3 dynamicLightPosition;
	float dynamicLightStrength;
};