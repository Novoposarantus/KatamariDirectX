#include "Light.h"

	bool Light::Initialize(float screenNear, float screenDepth)
	{
		this->SetPosition(20.0f, 20.0f, 20.0f);
		this->SetAmbientColor(1.0f, 1.0f, 1.0f, 0.3f);
		this->SetDiffuseColor(1.0f, 1.0f, 1.0f, 200.0f);
		this->SetLookAt(0.0f, 0.0f, 0.0f);
		this->SetAttenuation(1.0f, 0.045f, 0.045f);
		this->GenerateViewMatrix();
		this->GenerateProjectionMatrix(screenNear, screenDepth);
		return true;
	}

	void Light::SetAmbientColor(float red, float green, float blue, float strength)
	{
		this->ambientColor = Vector3(red, green, blue);
		this->ambientStrength = strength;
	}

	void Light::SetDiffuseColor(float red, float green, float blue, float strength)
	{
		this->diffuseColor = Vector3(red, green, blue);
		this->diffuseStrength = strength;
	}

	void Light::SetAttenuation(float a, float b, float c)
	{
		this->attenuation = Vector3(a, b, c);
	}

	void Light::SetPosition(float x, float y, float z)
	{
		this->pos = Vector3(x, y, z);
	}

	void Light::AdjustPosition(float x, float y, float z)
	{
		this->pos += Vector3(x, y, z);
	}

	void Light::SetLookAt(float x, float y, float z)
	{
		this->lookAt = Vector3(x, y, z);
	}

	Vector3 Light::GetAmbientColor()
	{
		return this->ambientColor;
	}

	float Light::GetAmbientStrength()
	{
		return this->ambientStrength;
	}

	Vector3 Light::GetDiffuseColor()
	{
		return this->diffuseColor;
	}

	float Light::GetDiffuseStrength()
	{
		return this->diffuseStrength;
	}

	Vector3 Light::GetAttenuation()
	{
		return this->attenuation;
	}

	Vector3 Light::GetPosition()
	{
		return this->pos;
	}

	Matrix Light::GetViewMatrix()
	{
		return this->viewMatrix;
	}

	Matrix Light::GetProjectionMatrix()
	{
		return this->projMatrix;
	}

	void Light::GenerateViewMatrix()
	{
		Vector3 camPos = Vector3(pos.x, pos.y, pos.z);
		Vector3 camLookAt = Vector3(lookAt.x, lookAt.y, lookAt.z);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

		viewMatrix = Matrix::CreateLookAt(camPos, camLookAt, up);
	}

	void Light::GenerateProjectionMatrix(float screenNear, float screenDepth)
	{
		float fieldOfView = (float)XM_PI / 2.0f;
		float screenAspect = 1.0f;
		this->projMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, screenNear, screenDepth);
	}


