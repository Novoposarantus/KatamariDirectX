#pragma once
#include "GameObject.h"

class RenderableGameObject : public GameObject
{
public:
	bool Initialize(
		const std::string& filePath, 
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext
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
	void SetRotation(const Vector3& rot) override;
	void SetRotation(float x, float y, float z) override;

	void Draw(ConstantBuffer<CB_VS_Mesh_Transform>& cb_vs_vertexshader);

protected:
	void UpdateWorldMatrix() override;
	float rotationSpeed = 0.005f;
	float size = 1;
	Model model;
	Matrix rotMatrix;
	RenderableGameObject* mainGameObject = nullptr;
	Vector3 mainObjectR;

};