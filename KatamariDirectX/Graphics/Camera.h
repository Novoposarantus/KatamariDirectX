#pragma once
#include <DirectXMath.h>

#include "GameObject.h"
using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetParent(GameObject* mainGameObject);
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const;
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const;
	void Rotation(float x, float y);
	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetRoll(float roll);
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
	void UpdateViewMatrix();
private:
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	GameObject* mainGameObject;

	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;

	float rotationSpeed = 0.01;
};

//class Camera
//{
//public:
//	Camera();
//	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
//
//	const DirectX::SimpleMath::Matrix& GetViewMatrix() const;
//	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const;
//
//	const XMVECTOR& GetPositionVector() const;
//	const DirectX::SimpleMath::Vector3& GetPositionFloat3() const;
//	const XMVECTOR& GetRotationVector() const;
//	const DirectX::SimpleMath::Vector3& GetRotationFloat3() const;
//
//	void SetPosition(const XMVECTOR& pos);
//	void SetPosition(const DirectX::SimpleMath::Vector3& pos);
//	void SetPosition(float x, float y, float z);
//	void AdjustPosition(const XMVECTOR& pos);
//	void AdjustPosition(const DirectX::SimpleMath::Vector3& pos);
//	void AdjustPosition(float x, float y, float z);
//	void SetRotation(const XMVECTOR& rot);
//	void SetRotation(const DirectX::SimpleMath::Vector3& rot);
//	void SetRotation(float x, float y, float z);
//	void AdjustRotation(const XMVECTOR& rot);
//	void AdjustRotation(const DirectX::SimpleMath::Vector3& rot);
//	void AdjustRotation(float x, float y, float z);
//	void SetLookAtPos(DirectX::SimpleMath::Vector3 lookAtPos);
//	const XMVECTOR& GetForwardVector();
//	const XMVECTOR& GetRightVector();
//	const XMVECTOR& GetBackwardVector();
//	const XMVECTOR& GetLeftVector();
//private:
//	void UpdateViewMatrix();
//	XMVECTOR posVector;
//	XMVECTOR rotVector;
//	DirectX::SimpleMath::Vector3 pos;
//	DirectX::SimpleMath::Vector3 rot;
//	DirectX::SimpleMath::Matrix viewMatrix;
//	DirectX::SimpleMath::Matrix projectionMatrix;
//
//	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
//	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
//	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//
//	XMVECTOR vec_forward;
//	XMVECTOR vec_left;
//	XMVECTOR vec_right;
//	XMVECTOR vec_backward;
//};