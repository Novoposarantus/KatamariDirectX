#pragma once
#include <DirectXMath.h>


struct CB_VS_VertexShader
{
	DirectX::SimpleMath::Matrix mat;
};

struct CB_PS_Alpha
{
	float alpha = 1.0f;
};

struct CB_PS_Light
{
	DirectX::SimpleMath::Vector3 ambientLightColor;
	float ambientLightStrength;
};