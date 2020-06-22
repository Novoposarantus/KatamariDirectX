#include "PointLight.h"

	bool PointLight::Initialize(float screenNear, float screenDepth)
	{
		this->SetPosition(20.0f, 20.0f, 20.0f);
		this->SetColor(1.0f, 1.0f, 1.0f, 200.0f);
		this->SetLookAt(0.0f, 0.0f, 0.0f);
		this->SetAttenuation(1.0f, 0.045f, 0.045f);
		this->GenerateViewMatrix();
		this->GenerateProjectionMatrix(screenNear, screenDepth);
		return true;
	}

	void PointLight::SetColor(float red, float green, float blue, float strength)
	{
		this->lightColor = Vector3(red, green, blue);
		this->lightStrength = strength;
	}

	void PointLight::SetAttenuation(float a, float b, float c)
	{
		this->attenuation = Vector3(a, b, c);
	}

	void PointLight::SetPosition(float x, float y, float z)
	{
		this->pos = Vector3(x, y, z);
	}

	void PointLight::AdjustPosition(float x, float y, float z)
	{
		this->pos += Vector3(x, y, z);
	}

	void PointLight::SetLookAt(float x, float y, float z)
	{
		this->lookAt = Vector3(x, y, z);
	}

	Vector3 PointLight::GetDiffuseColor()
	{
		return this->lightColor;
	}

	float PointLight::GetDiffuseStrength()
	{
		return this->lightStrength;
	}

	Vector3 PointLight::GetAttenuation()
	{
		return this->attenuation;
	}

	Vector3 PointLight::GetPosition()
	{
		return this->pos;
	}

	Matrix PointLight::GetViewMatrix()
	{
		return this->viewMatrix;
	}

	Matrix PointLight::GetProjectionMatrix()
	{
		return this->projMatrix;
	}

	void PointLight::GenerateViewMatrix()
	{
		Vector3 camPos = Vector3(pos.x, pos.y, pos.z);
		Vector3 camLookAt = Vector3(lookAt.x, lookAt.y, lookAt.z);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

		viewMatrix = Matrix::CreateLookAt(camPos, camLookAt, up);
	}

	void PointLight::GenerateProjectionMatrix(float screenNear, float screenDepth)
	{
		float fieldOfView = (float)XM_PI / 2.0f;
		float screenAspect = 1.0f;
		this->projMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, screenNear, screenDepth);
	}


