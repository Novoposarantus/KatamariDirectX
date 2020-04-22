#include "RenderableGameObject.h"
#include "..\\Math.h"

bool RenderableGameObject::Initialize(
	const std::string& filePath, 
	ID3D11Device* device, 
	ID3D11DeviceContext* deviceContext
)
{
	if (!model.Initialize(filePath, device, deviceContext))
		return false;

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->rotMatrix = Matrix::CreateFromYawPitchRoll(rot.x, rot.y, rot.z);
	this->UpdateWorldMatrix();
	return true;
}

void RenderableGameObject::Draw(ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader, const Matrix& viewProjectionMatrix)
{
	if (this->IsAttachedToMain())
	{
		this->UpdateWorldMatrix();
	}
	model.Draw(cb_vs_vertexshader, this->worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::Draw(ConstantBuffer<CB_VS_DEPTH>& cb_vs_vertexshader, const Matrix& viewProjectionMatrix)
{
	if (this->IsAttachedToMain())
	{
		this->UpdateWorldMatrix();
	}
	model.Draw(cb_vs_vertexshader, this->worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateWorldMatrix()
{
	if (!this->IsAttachedToMain())
	{
		this->worldMatrix = Matrix::CreateScale(this->scale * size) * rotMatrix;
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
			* this->mainGameObject->rotMatrix
			* transToWorld;
	}
	this->UpdateDirectionVectors();
}


void RenderableGameObject::AttachToMain(RenderableGameObject* mainObject)
{
	auto matrix = mainObject->GetWorldMatrix().Invert();
	auto vector = Vector3::Transform(this->pos, matrix);
	this->pos = vector;
	this->mainGameObject = mainObject;
}

bool RenderableGameObject::IsAttachedToMain()
{
	return this->mainGameObject != nullptr;
}

float RenderableGameObject::GetSize()
{
	return this->size;
}

void RenderableGameObject::SetRotation(const Vector3& rot)
{
	this->rotMatrix = Matrix::CreateFromYawPitchRoll(rot.x, rot.y, rot.z);
	this->UpdateWorldMatrix();
}

void RenderableGameObject::SetRotation(float x, float y, float z)
{
	this->rotMatrix = Matrix::CreateFromYawPitchRoll(x, y, z);
	this->UpdateWorldMatrix();
}

const Vector3 RenderableGameObject::GetMaxDirection()
{
	return MulVector3(this->model.GetMaxDirections(), this->scale * this->size) + this->pos;
}

const Vector3 RenderableGameObject::GeMinDirection()
{
	return MulVector3(this->model.GetMinDirections(), this->scale * this->size) + this->pos;
}

void RenderableGameObject::SetSize(float size)
{
	this->size = size;
	this->UpdateWorldMatrix();
}

const bool RenderableGameObject::CheckColision(RenderableGameObject& gameObject)
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
		return true;
	};
	return false;
}

const bool RenderableGameObject::CheckFutureColision(RenderableGameObject& gameObject, Vector3 adjustPosition)
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
		return true;
	};
	return false;
}

const bool RenderableGameObject::CanAttach(float curSize)
{
	return curSize >= this->size;
}


void RenderableGameObject::Rotate(Vector3 rot, float dt)
{
	rotMatrix *= Matrix::CreateFromAxisAngle(rot, rotationSpeed * dt);
	Vector3 scale(0, 0, 0);
	Quaternion rotQ(0, 0, 0, 0);
	Vector3 trans(0, 0, 0);
	rotMatrix.Decompose(scale, rotQ, trans);
	this->rot = Vector3(rotQ.x, rotQ.y, rotQ.z);
	this->UpdateWorldMatrix();
}