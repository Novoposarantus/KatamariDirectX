#pragma once
#include "GameObject.h"

class RenderableGameObject : public GameObject
{
public:
	bool Initialize(
		const std::string& filePath, 
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext, 
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader
	);

	const Vector3 GetMaxDirection();
	const Vector3 GeMinDirection();
	void SetSize(float size);
	const bool CheckColision(RenderableGameObject& gameObject);
	const bool CheckFutureColision(RenderableGameObject& gameObject, Vector3 adjustPosition);
	const bool CanAttach(float curSize);
	void Rotate(Vector3 rot, float dt);
	void AttachToMain(RenderableGameObject* mainObject);
	bool IsAttachedToMain();
	float GetSize();

	void Draw(const Matrix& viewProjectionMatrix);

protected:
	float rotationSpeed = 0.005f;
	float size = 1;
	Model model;
	Matrix rotMatirx;
	RenderableGameObject* mainGameObject = nullptr;
	Vector3 mainObjectR;

	void UpdateWorldMatrix() override;
};