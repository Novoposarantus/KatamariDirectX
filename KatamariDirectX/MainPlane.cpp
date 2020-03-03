#include "MainPlane.h"
#include "Macros.h"


bool MainPlane::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	if (!InitializeShaders(device))
		return false;

	if (!InitializeScene(device))
		return false;

	return true;
}

void MainPlane::RenderFrame(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
}

bool MainPlane::InitializeShaders(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	ShaderFolder();

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(layoutDesc);

	if (!vertexshader.Initialize(
		device,
		shaderFolder + L"MainPlaneVS.cso",
		layoutDesc,
		numElements))
	{
		return false;
	}

	if (!pixelshader.Initialize(device, shaderFolder + L"MainPlanePS.cso"))
	{
		return false;
	}

	return true;
}

bool MainPlane::InitializeScene(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	MainPlaneVertex v[] =
	{
		MainPlaneVertex(-100,	-1, -100, 1.0f, 1.0f, 1.0f), //Bottom Left 
		MainPlaneVertex(-100,	-1,  100, 1.0f, 1.0f, 1.0f), //Top Left
		MainPlaneVertex(100,	-1,  100, 1.0f, 1.0f, 1.0f), //Top Right
		MainPlaneVertex(100,	-1, -100, 1.0f, 1.0f, 1.0f), //Bottom Right
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(device.Get(), v, ARRAYSIZE(v));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}


	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(device.Get(), indices, ARRAYSIZE(indices));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create indices buffer.");
		return false;
	}


	return true;
}
