#pragma once
#include "RenderableGameObject.h"

class PointLight
{
public:
	bool Initialize(float, float);
	void SetColor(float, float, float, float);
	void SetAttenuation(float, float, float);
	void SetPosition(float, float, float);
	void AdjustPosition(float, float, float);
	void SetLookAt(float, float, float);

	Vector3 GetDiffuseColor();
	float GetDiffuseStrength();
	Vector3 GetAttenuation();
	Vector3 GetPosition();

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();


	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);
private:
	Vector3 lightColor;
	float lightStrength;
	Vector3 attenuation;
	Vector3 pos;
	Vector3 lookAt;

	Matrix viewMatrix;
	Matrix projMatrix;
};