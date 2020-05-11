#include "RenderTarget.h"


RenderTarget::RenderTarget()
{
	m_RTTexture = nullptr;
	m_RTV = nullptr;
	m_SRV = nullptr;
	m_DSTexture = nullptr;
	m_DSV = nullptr;
}

bool RenderTarget::Init(ID3D11Device* device, float screenNear, float screenDepth)
{
	// Сначала мы создаем текстуру в которую будем выводить shadow map

	// создаем текстуру размером SHADOWMAP_WIDTH х SHADOWMAP_HEIGHT
	// данная текстура будет использоваться как render target (установили
	// флаг - D3D11_BIND_RENDER_TARGET)
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = SHADOWMAP_WIDTH;
	textureDesc.Height = SHADOWMAP_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	HRESULT hr;
	hr = device->CreateTexture2D(&textureDesc, NULL, m_RTTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D");

	// Создаем render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_RTTexture.Get(), &renderTargetViewDesc, m_RTV.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateRenderTargetView");

	// создаем shader resource view c ранее созданной текстуры
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_RTTexture.Get(), &shaderResourceViewDesc, m_SRV.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateShaderResourceView");

	// Создаем Depth Stencil View

	// Создаем текстуру глубины
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = SHADOWMAP_WIDTH;
	depthBufferDesc.Height = SHADOWMAP_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, m_DSTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D depth");

	// создаем depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(m_DSTexture.Get(), &depthStencilViewDesc, m_DSV.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateDepthStencilView");

	// Задаем вьюпорт по размеру нашей текстуры
	m_viewport.Width = (float)SHADOWMAP_WIDTH;
	m_viewport.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Создаем проекционную матрицу
	m_projectionMatrix = XMMatrixPerspectiveFovLH(((float)XM_PI / 4.0f), ((float)SHADOWMAP_WIDTH / (float)SHADOWMAP_HEIGHT), screenNear, screenDepth);

	return true;
}

void RenderTarget::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	// Биндим RTV и буфер глубины к конвееру
	deviceContext->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());
	// Устанавливаем вьюпорт
	deviceContext->RSSetViewports(1, &m_viewport);
}

void RenderTarget::ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };
	deviceContext->ClearRenderTargetView(m_RTV.Get(), color);
	deviceContext->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTarget::GetShaderResourceView()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** RenderTarget::GetShaderResourceViewAddress()
{
	return m_SRV.GetAddressOf();
}

XMMATRIX RenderTarget::GetProjectionMatrix()
{
	return m_projectionMatrix;
}