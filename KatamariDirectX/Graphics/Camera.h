#pragma once
#include <DirectXMath.h>

#include "RenderableGameObject.h"
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera();
	void SetParent(RenderableGameObject* mainGameObject);
	const Matrix& GetViewMatrix() const;
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	const Matrix& GetProjectionMatrix() const;
	Vector3 GetPosotion();
	Vector3 GetRotation();
	const float GetSpeed();
	void Rotation(float x, float y);
	void UpdateViewMatrix();

	const Vector3& GetForwardVector();
	const Vector3& GetRightVector();
	const Vector3& GetBackwardVector();
	const Vector3& GetLeftVector();

	const float rotationSpeed = 0.001f;


	float r = 20.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

private:
	RenderableGameObject* mainGameObject;

	Matrix viewMatrix;
	Matrix projectionMatrix;

	const Vector3 DEFAULT_FORWARD_VECTOR = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 DEFAULT_UP_VECTOR = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 DEFAULT_BACKWARD_VECTOR = Vector3(0.0f, 0.0f, -1.0f);
	const Vector3 DEFAULT_LEFT_VECTOR = Vector3(-1.0f, 0.0f, 0.0f);
	const Vector3 DEFAULT_RIGHT_VECTOR = Vector3(1.0f, 0.0f, 0.0f);

	Vector3 vec_forward;
	Vector3 vec_left;
	Vector3 vec_right;
	Vector3 vec_backward;
	Vector3 pos;

	float transSpeed = 0.01f;
};