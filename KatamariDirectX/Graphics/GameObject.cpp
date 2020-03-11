#include "GameObject.h"
#include "..\\Math.h"

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
	this->SetScale(1.0f, 1.0f, 1.0f, 1.0f);
	this->rotMatirx = Matrix::CreateFromYawPitchRoll(rot.x, rot.y, rot.z);
	this->UpdateWorldMatrix();
	return true;
}

void GameObject::Draw(const Matrix& viewProjectionMatrix)
{
	if (this->IsAttachedToMain())
	{
		this->UpdateWorldMatrix();
	}
	model.Draw(this->worldMatrix, viewProjectionMatrix);
}

const Matrix& GameObject::GetWorldMatrix() const
{
	return this->worldMatrix;
}

void GameObject::UpdateWorldMatrix()
{
	if (!this->IsAttachedToMain())
	{
		this->worldMatrix = Matrix::CreateScale(this->scale * size) * rotMatirx;//Matrix::CreateFromYawPitchRoll(this->rot.y, this->rot.x, this->rot.z);
		this->worldMatrix.Translation(Vector3(this->pos.x, this->pos.y, this->pos.z));
	}
	else 
	{
		auto transToWorld = Matrix::Identity;
		transToWorld.Translation(this->mainGameObject->GetPosition());
		auto transToLocal = Matrix::Identity;
		transToLocal.Translation(this->pos);	
		this->worldMatrix = 
			Matrix::Identity 
			* Matrix::CreateScale(this->scale * size) 
			* transToLocal
			* this->mainGameObject->rotMatirx
			* transToWorld;
	}

	Matrix vecRotationMatrix = Matrix::CreateFromYawPitchRoll(this->rot.y, 0.0f, 0.0f);
	this->vec_forward = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);\
	this->vec_left = Vector3::Transform(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
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

void GameObject::SetScale(const Vector3& scale, float size)
{
	this->scale = scale;
	this->size = size;
	this->UpdateWorldMatrix();
}

void GameObject::SetScale(float x, float y, float z, float size)
{
	this->scale = Vector3(x, y, z);
	this->size = size;
	this->UpdateWorldMatrix();
}

void GameObject::Rotate(Vector3 rot, float dt)
{
	rotMatirx *= Matrix::CreateFromAxisAngle(rot, rotationSpeed * dt);
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

const Vector3 GameObject::GetMaxDirection()
{
	return MulVector3(this->model.GetMaxDirections(), this->scale * this->size) + this->pos;
}

const Vector3 GameObject::GeMinDirection()
{
	return MulVector3(this->model.GetMinDirections(), this->scale * this->size) + this->pos;
}

const bool GameObject::CheckColision(GameObject& gameObject)
{
	auto maxDirOwn = this->GetMaxDirection();
	auto minDirOwn = this->GeMinDirection();
	auto maxDirTo = gameObject.GetMaxDirection();
	auto minDirTo = gameObject.GeMinDirection();
	if (maxDirTo.x >= minDirOwn.x
		&& maxDirTo.z >= minDirOwn.z
		&& minDirTo.x <= maxDirOwn.x
		&& minDirTo.z <= maxDirOwn.z)
	{
		this->mainObjectR =  this->pos - gameObject.pos;
		return true;
	};
	return false;
}

const bool GameObject::CheckFutureColision(GameObject& gameObject, Vector3 adjustPosition)
{
	auto maxDirOwn = this->GetMaxDirection();
	auto minDirOwn = this->GeMinDirection();
	auto maxDirTo = gameObject.GetMaxDirection() + adjustPosition;
	auto minDirTo = gameObject.GeMinDirection() + adjustPosition;
	if (maxDirTo.x >= minDirOwn.x
		&& maxDirTo.z >= minDirOwn.z
		&& minDirTo.x <= maxDirOwn.x
		&& minDirTo.z <= maxDirOwn.z)
	{
		this->mainObjectR = this->pos - gameObject.pos;
		return true;
	};
	return false;
}

const bool GameObject::CanAttach(float curSize)
{
	return curSize >= this->size;
}

const Vector3& GameObject::GetForwardVector()
{
	return this->vec_forward;
}

const Vector3& GameObject::GetLeftVector()
{
	return this->vec_left;
}

void GameObject::AttachToMain(GameObject* mainObject)
{
	auto matrix =  mainObject->GetWorldMatrix().Invert();
	auto vector = Vector3::Transform(this->pos, matrix);
	this->pos = vector;
	this->mainGameObject = mainObject;
}

bool GameObject::IsAttachedToMain()
{
	return this->mainGameObject != nullptr;
}
