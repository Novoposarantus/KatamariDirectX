#include "GameObject.h"

bool GameObject::Initialize(
	const std::string& filePath,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
	if (!model.Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
		return false;

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->SetScale(1.0f, 1.0f, 1.0f);
	this->UpdateWorldMatrix();
	return true;
}

const Matrix& GameObject::GetWorldMatrix() const
{
	return this->worldMatrix;
}

void GameObject::UpdateWorldMatrix()
{
	assert("UpdateMatrix must be overridden." && 0);
}

void GameObject::UpdateDirectionVectors()
{
	Matrix vecRotationMatrix = Matrix::CreateFromYawPitchRoll(this->rot.y, this->rot.x, 0.0f);
	this->vec_forward = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_left = Vector3::Transform(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);

	Matrix vecRotationMatrixNoY = Matrix::CreateFromYawPitchRoll(0.0f, this->rot.x, 0.0f);
	this->vec_forward_noY = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_left_noY = Vector3::Transform(this->DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
}

const Vector3& GameObject::GetPosition() const
{
	return this->pos;
}

const Vector3& GameObject::GetRotation() const
{
	return this->rot;
}


void GameObject::SetPosition(const Vector3& pos)
{
	this->pos = pos;
	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = Vector3(x, y, z);
	this->UpdateWorldMatrix();
}


void GameObject::AdjustPosition(const Vector3& pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const Vector3& rot)
{
	this->rot = rot;
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rot = Vector3(x, y, z);
	this->UpdateWorldMatrix();
}

void GameObject::SetScale(const Vector3& scale)
{
	this->scale = scale;
	this->UpdateWorldMatrix();
}

void GameObject::SetScale(float x, float y, float z)
{
	this->scale = Vector3(x, y, z);
	this->UpdateWorldMatrix();
}



void GameObject::AdjustRotation(const Vector3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->UpdateWorldMatrix();
}

const Vector3& GameObject::GetForwardVector(bool omitY)
{
	if (omitY)
		return this->vec_forward_noY;
	return  this->vec_forward;
}

const Vector3& GameObject::GetLeftVector(bool omitY)
{
	if (omitY)
		return this->vec_left_noY;
	return this->vec_left;
}
