#pragma once
#include "Model.h"

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	);
	const Matrix& GetWorldMatrix() const;
	virtual const Vector3& GetPosition() const;
	virtual const Vector3& GetRotation() const;

	virtual void SetPosition(const Vector3& pos);
	virtual void SetPosition(float x, float y, float z);
	virtual void AdjustPosition(const Vector3& pos);
	virtual void AdjustPosition(float x, float y, float z);
	virtual void SetRotation(const Vector3& rot);
	virtual void SetRotation(float x, float y, float z);
	virtual void SetScale(const Vector3& rot);
	virtual void SetScale(float x, float y, float z);
	virtual void AdjustRotation(const Vector3& rot);
	virtual void AdjustRotation(float x, float y, float z);
	const Vector3& GetForwardVector(bool omitY = false);
	const Vector3& GetLeftVector(bool omitY = false);

	Model model;

protected:
	virtual void UpdateWorldMatrix();
	void UpdateDirectionVectors();

	Matrix worldMatrix = Matrix::Identity;

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	Vector3 vec_forward;
	Vector3 vec_left;


	Vector3 vec_forward_noY;
	Vector3 vec_left_noY;
};