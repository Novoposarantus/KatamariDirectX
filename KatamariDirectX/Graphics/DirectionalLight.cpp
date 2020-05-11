#include "DirectionalLight.h"

bool DirectionalLight::Initialize(float screenNear, float screenDepth)
{
	this->SetDirectionalColor(1.0f, 1.0f, 1.0f, 1.0f);
	this->SetDirection(0.5, -1, 0);
	this->GenerateProjectionMatrix(screenNear, screenDepth);
	return true;
}

void DirectionalLight::SetDirectionalColor(float red, float green, float blue, float strength)
{
	this->directionalColor = Vector3(red, green, blue);
	this->directionalStrength = strength;
}

void DirectionalLight::SetDirection(float x, float y, float z)
{
	this->dir = Vector3(x, y, z);
}

Vector3 DirectionalLight::GetDirectionalColor()
{
	return this->directionalColor;
}

float DirectionalLight::GetDirectionalStrength()
{
	return this->directionalStrength;
}

Vector3 DirectionalLight::GetDirection()
{
	return this->dir;
}

Matrix DirectionalLight::GetViewMatrix()
{
	return this->viewMatrix;
}

Matrix DirectionalLight::GetProjectionMatrix()
{
	return this->projMatrix;
}

void DirectionalLight::UpdateViewMatrix(Vector3 camPos)
{
	float ShadowDinX = 10;
	float ShadowDinY = ShadowDinX;
	float ShadowDinZ = 30;

	Vector3 shadowCamPos = camPos - this->dir * 10;

	this->viewMatrix = Matrix::CreateLookAt(
		shadowCamPos,
		camPos,
		Vector3::Up
	);
}

void DirectionalLight::GenerateProjectionMatrix(float screenNear, float screenDepth)
{
	float fieldOfView = (float)XM_PI / 2.0f;
	float screenAspect = 1.0f;
	this->projMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, screenNear, screenDepth);
}