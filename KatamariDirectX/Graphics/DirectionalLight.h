#pragma once
#include "RenderableGameObject.h"

class DirectionalLight
{
public:
	bool Initialize(float, float);
	void SetDirectionalColor(float, float, float, float);
	void SetDirection(float, float, float);

	Vector3 GetDirectionalColor();
	float GetDirectionalStrength();
	Vector3 GetDirection();

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

	void UpdateViewMatrix(Vector3);
	void AdjustTime(float);

private:
	void GenerateProjectionMatrix(float, float);
	bool isNight;

	Vector3 directionalColor;
	float directionalStrength;
	Vector3 dir;

	Matrix viewMatrix;
	Matrix projMatrix;
};