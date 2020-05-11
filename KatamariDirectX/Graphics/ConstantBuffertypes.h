#pragma once
#include <DirectXMath.h>


struct CB_VS_VertexShader
{
	DirectX::SimpleMath::Matrix wvpMatrix;
	DirectX::SimpleMath::Matrix worldMatrix;
};

struct CB_VS_Mesh_Transform 
{
	DirectX::SimpleMath::Matrix worldMatrix;
	DirectX::SimpleMath::Matrix meshTransformMatrix;
};

struct CB_VS_DEPTH
{
	DirectX::SimpleMath::Matrix camShadowViewMatrix;
	DirectX::SimpleMath::Matrix camShadowProjMatrix;
};

struct CB_VS_Cam
{
	DirectX::SimpleMath::Matrix camViewMatrix;
	DirectX::SimpleMath::Matrix camProjMatrix;
	DirectX::SimpleMath::Matrix camShadowViewMatrix;
	DirectX::SimpleMath::Matrix camShadowProjMatrix;
};

struct CB_PS_Alpha
{
	float alpha = 1.0f;
};

struct CB_PS_Light
{
	DirectX::SimpleMath::Vector3 directionalLightColor;
	float directionalLightStrenght;
	DirectX::SimpleMath::Vector3 directionalLightDir;
	float dirPadding;
	DirectX::SimpleMath::Vector3 camPos;
	float posPadding;
};
