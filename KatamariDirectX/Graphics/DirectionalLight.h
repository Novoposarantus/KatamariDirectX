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

private:
	void GenerateProjectionMatrix(float, float);


	Vector3 directionalColor;
	float directionalStrength;
	Vector3 dir;

	Matrix viewMatrix;
	Matrix projMatrix;
};