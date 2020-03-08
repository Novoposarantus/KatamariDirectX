#pragma once
#include "Model.h"

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader
	);
	void Draw(const Matrix& viewProjectionMatrix);
	const Matrix& GetWorldMatrix() const;
	const Vector3& GetPosition() const;
	const Vector3& GetRotation() const;

	void SetPosition(const Vector3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const Vector3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const Vector3& rot);
	void SetRotation(float x, float y, float z);
	void SetScale(const Vector3& rot, float size = 1);
	void SetScale(float x, float y, float z, float size = 1);
	void AdjustRotation(const Vector3& rot);
	void AdjustRotation(float x, float y, float z);
	const Vector3 GetMaxDirection();
	const Vector3 GeMinDirection();
	const bool CheckColision(GameObject& gameObject);
	const bool CheckFutureColision(GameObject& gameObject, Vector3 adjustPosition);
	const bool CanAttach(float curSize);
	const Vector3& GetForwardVector();
	const Vector3& GetLeftVector();
	void AttachToMain(GameObject* mainObject);
	bool IsAttachedToMain();

	Model model;
	float rotationSpeed = -0.005f;
	float size = 1;
private:
	void UpdateWorldMatrix();

	Matrix worldMatrix = XMMatrixIdentity();
	GameObject* mainGameObject = nullptr;
	Vector3 mainObjectR;

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	const Vector3 DEFAULT_FORWARD_VECTOR = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 DEFAULT_UP_VECTOR = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 DEFAULT_LEFT_VECTOR = Vector3(-1.0f, 0.0f, 0.0f);

	Vector3 vec_forward;
	Vector3 vec_left;


};