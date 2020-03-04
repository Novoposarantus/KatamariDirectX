#include "MainPlane.h"
#include "..\\..\\Macros.h"

bool MainPlane::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->texture = texture;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;

	try
	{
		//Textured Square
		Vertex v[] =
		{
			Vertex(-100,  -0.5f, -100, 0.0f, 1.0f), //FRONT Bottom Left   - [0]
			Vertex(-100,  -0.5f,  100, 0.0f, 0.0f), //FRONT Top Left      - [1]
			Vertex( 100,  -0.5f,  100, 1.0f, 0.0f), //FRONT Top Right     - [2]
			Vertex( 100,  -0.5f, -100, 1.0f, 1.0f), //FRONT Bottom Right   - [3]
		};

		//Load Vertex Data
		HRESULT hr = this->vertexBuffer.Initialize(this->device, v, ARRAYSIZE(v));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

		DWORD indices[] =
		{
			0, 1, 2,
			0, 2, 3, 
		};

		//Load Index Data
		hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

void MainPlane::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

void MainPlane::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	this->cb_vs_vertexshader->data.mat = this->worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->cb_vs_vertexshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

	this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture
	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->DrawIndexed(this->indexBuffer.IndexCount(), 0, 0); //Draw
}
