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

	const Vector3& GetForwardVector(bool omitPitch = false);
	const Vector3& GetLeftVector(bool omitPitch = false);

	const float rotationSpeed = 0.001f;


	float r = 20.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

private:
	void UpdateDirectionVectors();
	RenderableGameObject* mainGameObject;

	Matrix viewMatrix;
	Matrix projectionMatrix;


	Vector3 vec_forward;
	Vector3 vec_left;

	Vector3 vec_forward_no_pitch;
	Vector3 vec_left_no_pitch;

	Vector3 pos;

	float transSpeed = 0.01f;
};