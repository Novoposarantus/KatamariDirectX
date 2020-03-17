#pragma once
#include "Model.h"

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader
	);
	const Matrix& GetWorldMatrix() const;
	const Vector3& GetPosition() const;
	const Vector3& GetRotation() const;

	void SetPosition(const Vector3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const Vector3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const Vector3& rot);
	void SetRotation(float x, float y, float z);
	void SetScale(const Vector3& rot);
	void SetScale(float x, float y, float z);
	void AdjustRotation(const Vector3& rot);
	void AdjustRotation(float x, float y, float z);
	const Vector3& GetForwardVector(bool omitY);
	const Vector3& GetLeftVector(bool omitY);

	Model model;
protected:
	virtual void UpdateWorldMatrix();
	void UpdateDirectionVectors();

	Matrix worldMatrix = Matrix::Identity;

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	const Vector3 DEFAULT_FORWARD_VECTOR = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 DEFAULT_UP_VECTOR = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 DEFAULT_LEFT_VECTOR = Vector3(-1.0f, 0.0f, 0.0f);

	Vector3 vec_forward;
	Vector3 vec_left;


	Vector3 vec_forward_noY;
	Vector3 vec_left_noY;
};