#pragma once
#include "..\\Vertex.h"
#include "..\\VertexBuffer.h"
#include "..\\IndexBuffer.h"
#include "..\\ConstantBuffer.h"


using namespace DirectX;

class MainPlane
{
public:
	bool Initialize(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader
	);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const DirectX::SimpleMath::Matrix& viewProjectionMatrix);

private:

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_VertexShader>* cb_vs_vertexshader = nullptr;
	ID3D11ShaderResourceView* texture = nullptr;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

	DirectX::SimpleMath::Matrix worldMatrix = XMMatrixIdentity();
};