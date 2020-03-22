#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
	r = 4.0f;
	pitch = 20.0f;
	yaw = 0.0f;
	this->UpdateViewMatrix();
}

void Camera::SetParent(RenderableGameObject* mainGameObject)
{
	this->mainGameObject = mainGameObject;
}

const Matrix& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const Matrix& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

Vector3 Camera::GetPosotion()
{
	return this->pos;
}

Vector3 Camera::GetRotation()
{
	Vector3 scale(0, 0, 0);
	Quaternion rot(0, 0, 0, 0);
	Vector3 trans(0, 0, 0);
	this->viewMatrix.Decompose(scale, rot, trans);
	return Vector3(rot.x, rot.y, rot.z);
}

void Camera::Rotation(float dx, float dy)
{
	this->yaw -= dx * rotationSpeed;
	this->UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	Vector3 personPos(0, 0, 0);
	if (mainGameObject != nullptr)
	{
		auto mainGameObjectWorldMatrix = mainGameObject->GetWorldMatrix();
		personPos = mainGameObjectWorldMatrix.Translation();
	}
	auto mat = Matrix::CreateFromYawPitchRoll(this->yaw, this->pitch, 0);
	this->pos = Vector3::Transform(Vector3(0, r, 0), mat) + personPos;
	this->viewMatrix = Matrix::CreateLookAt(
		this->pos,
		personPos, 
	Vector3::Up);
	
	Matrix vecRotationMatrix = Matrix::CreateFromYawPitchRoll(this->yaw, 0.0f, 0.0f);
	this->vec_forward = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = Vector3::Transform(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_right = Vector3::Transform(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	this->vec_left = Vector3::Transform(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
}

const Vector3& Camera::GetForwardVector()
{
	return this->vec_forward;
}

const Vector3& Camera::GetRightVector()
{
	return this->vec_right;
}

const Vector3& Camera::GetBackwardVector()
{
	return this->vec_backward;
}

const Vector3& Camera::GetLeftVector()
{
	return this->vec_left;
}

const float Camera::GetSpeed() {
	return transSpeed;
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = Matrix::CreatePerspectiveFieldOfView(fovRadians, aspectRatio, nearZ, farZ);
}