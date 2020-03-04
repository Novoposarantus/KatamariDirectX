#pragma once
#include "Model.h"

class GameObject
{
public:
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader
	);
	void Draw(const DirectX::SimpleMath::Matrix& viewProjectionMatrix);
	const DirectX::SimpleMath::Matrix& GetWorldMatrix() const;
	const XMVECTOR& GetPositionVector() const;
	const DirectX::SimpleMath::Vector3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const DirectX::SimpleMath::Vector3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const DirectX::SimpleMath::Vector3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const DirectX::SimpleMath::Vector3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const DirectX::SimpleMath::Vector3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(const DirectX::SimpleMath::Vector3& rot);
	void AdjustRotation(float x, float y, float z);
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
private:
	Model model;
	void UpdateWorldMatrix();

	DirectX::SimpleMath::Matrix worldMatrix = XMMatrixIdentity();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 rot;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
};