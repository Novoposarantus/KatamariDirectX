#pragma once
#include "RenderableGameObject.h"

class Light : public RenderableGameObject
{
public:
	bool Initialize(
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext, 
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader);

	DirectX::SimpleMath::Vector3 lightColor = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
	float lightStrength = 200.0f;
	float attenuation_a = 1.0f;
	float attenuation_b = 0.045f;
	float attenuation_c = 0.0451f;
};