#include "Light.h"

bool Light::Initialize(
	ID3D11Device* device, 
	ID3D11DeviceContext* deviceContext, 
	ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
	if (!model.Initialize("Data/Objects/light.fbx", device, deviceContext, cb_vs_vertexshader))
		return false;

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->SetScale(1, 1, 1);
	this->UpdateWorldMatrix();
	return true;
}