#include "GBufferRT.h"


GBufferRT::GBufferRT()
{
	m_RTTexture_Normal = nullptr;
	m_RTV_Normal = nullptr;
	m_SRV_Normal = nullptr;
	m_DSTexture_Normal = nullptr;
	m_DSV_Normal = nullptr;

	m_RTTexture_Depth = nullptr;
	m_RTV_Depth = nullptr;
	m_SRV_Depth = nullptr;
	m_DSTexture_Depth = nullptr;
	m_DSV_Depth = nullptr;

	m_RTTexture_Ambient = nullptr;
	m_RTV_Ambient = nullptr;
	m_SRV_Ambient = nullptr;
	m_DSTexture_Ambient = nullptr;
	m_DSV_Ambient = nullptr;

	m_RTTexture_Texture = nullptr;
	m_RTV_Texture = nullptr;
	m_SRV_Texture = nullptr;
	m_DSTexture_Texture = nullptr;
	m_DSV_Texture = nullptr;
}

bool GBufferRT::Init(ID3D11Device* device, float screenNear, float screenDepth)
{
	if (!this->InitNormals(device)) {
		return false;
	}

	if (!this->InitDepth(device)) {
		return false;
	}

	if (!this->InitAmbient(device)) {
		return false;
	}

	if (!this->InitTexture(device)) {
		return false;
	}

	this->InitProjMatrix(screenNear, screenDepth);

	return true;
}

bool GBufferRT::InitNormals(ID3D11Device* device)
{
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
	hr = device->CreateTexture2D(&textureDesc, NULL, m_RTTexture_Normal.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D");

	// Создаем render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_RTTexture_Normal.Get(), &renderTargetViewDesc, m_RTV_Normal.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateRenderTargetView");

	// создаем shader resource view c ранее созданной текстуры
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_RTTexture_Normal.Get(), &shaderResourceViewDesc, m_SRV_Normal.GetAddressOf());
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
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, m_DSTexture_Normal.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D depth");

	// создаем depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(m_DSTexture_Normal.Get(), &depthStencilViewDesc, m_DSV_Normal.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateDepthStencilView");

	// Задаем вьюпорт по размеру нашей текстуры
	m_viewport_Normal.Width = (float)SHADOWMAP_WIDTH;
	m_viewport_Normal.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport_Normal.MinDepth = 0.0f;
	m_viewport_Normal.MaxDepth = 1.0f;
	m_viewport_Normal.TopLeftX = 0.0f;
	m_viewport_Normal.TopLeftY = 0.0f;

	return true;
}

bool GBufferRT::InitDepth(ID3D11Device* device)
{

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
	hr = device->CreateTexture2D(&textureDesc, NULL, m_RTTexture_Depth.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D");

	// Создаем render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_RTTexture_Depth.Get(), &renderTargetViewDesc, m_RTV_Depth.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateRenderTargetView");

	// создаем shader resource view c ранее созданной текстуры
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_RTTexture_Depth.Get(), &shaderResourceViewDesc, m_SRV_Depth.GetAddressOf());
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
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, m_DSTexture_Depth.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D depth");

	// создаем depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(m_DSTexture_Depth.Get(), &depthStencilViewDesc, m_DSV_Depth.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateDepthStencilView");

	// Задаем вьюпорт по размеру нашей текстуры
	m_viewport_Depth.Width = (float)SHADOWMAP_WIDTH;
	m_viewport_Depth.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport_Depth.MinDepth = 0.0f;
	m_viewport_Depth.MaxDepth = 1.0f;
	m_viewport_Depth.TopLeftX = 0.0f;
	m_viewport_Depth.TopLeftY = 0.0f;

	return true;
}

bool GBufferRT::InitAmbient(ID3D11Device* device)
{

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
	hr = device->CreateTexture2D(&textureDesc, NULL, m_RTTexture_Ambient.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D");

	// Создаем render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_RTTexture_Ambient.Get(), &renderTargetViewDesc, m_RTV_Ambient.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateRenderTargetView");

	// создаем shader resource view c ранее созданной текстуры
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_RTTexture_Ambient.Get(), &shaderResourceViewDesc, m_SRV_Ambient.GetAddressOf());
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
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, m_DSTexture_Ambient.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D depth");

	// создаем depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(m_DSTexture_Ambient.Get(), &depthStencilViewDesc, m_DSV_Ambient.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateDepthStencilView");

	// Задаем вьюпорт по размеру нашей текстуры
	m_viewport_Ambient.Width = (float)SHADOWMAP_WIDTH;
	m_viewport_Ambient.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport_Ambient.MinDepth = 0.0f;
	m_viewport_Ambient.MaxDepth = 1.0f;
	m_viewport_Ambient.TopLeftX = 0.0f;
	m_viewport_Ambient.TopLeftY = 0.0f;

	return true;
}

bool GBufferRT::InitTexture(ID3D11Device* device)
{

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
	hr = device->CreateTexture2D(&textureDesc, NULL, m_RTTexture_Texture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D");

	// Создаем render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_RTTexture_Texture.Get(), &renderTargetViewDesc, m_RTV_Texture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateRenderTargetView");

	// создаем shader resource view c ранее созданной текстуры
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_RTTexture_Texture.Get(), &shaderResourceViewDesc, m_SRV_Texture.GetAddressOf());
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
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, m_DSTexture_Texture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateTexture2D depth");

	// создаем depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(m_DSTexture_Texture.Get(), &depthStencilViewDesc, m_DSV_Texture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "RenderTarget CreateDepthStencilView");

	// Задаем вьюпорт по размеру нашей текстуры
	m_viewport_Texture.Width = (float)SHADOWMAP_WIDTH;
	m_viewport_Texture.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport_Texture.MinDepth = 0.0f;
	m_viewport_Texture.MaxDepth = 1.0f;
	m_viewport_Texture.TopLeftX = 0.0f;
	m_viewport_Texture.TopLeftY = 0.0f;

	return true;
}

bool GBufferRT::InitProjMatrix(float screenNear, float screenDepth)
{
	m_projectionMatrix = XMMatrixPerspectiveFovLH(((float)XM_PI / 4.0f), ((float)SHADOWMAP_WIDTH / (float)SHADOWMAP_HEIGHT), screenNear, screenDepth);
	return true;
}

void GBufferRT::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	ID3D11RenderTargetView* rtvs[4] = {
		m_RTV_Normal.Get(),
		m_RTV_Depth.Get(),
		m_RTV_Ambient.Get(),
		m_RTV_Texture.Get()
	};
	ID3D11DepthStencilView* dsvs[4] = {
		m_DSV_Normal.Get(),
		m_DSV_Depth.Get(),
		m_DSV_Ambient.Get(),
		m_DSV_Texture.Get()
	};
	D3D11_VIEWPORT* viewPorts[4] = {
		&m_viewport_Normal,
		&m_viewport_Depth,
		&m_viewport_Ambient,
		&m_viewport_Texture,
	};

	// Биндим RTV и буфер глубины к конвееру
	deviceContext->OMSetRenderTargets(4, &rtvs[0], dsvs[0]);
	// Устанавливаем вьюпорт
	deviceContext->RSSetViewports(4, viewPorts[0]);
}

void GBufferRT::ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };
	deviceContext->ClearRenderTargetView(m_RTV_Normal.Get(), color);
	deviceContext->ClearDepthStencilView(m_DSV_Normal.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->ClearRenderTargetView(m_RTV_Depth.Get(), color);
	deviceContext->ClearDepthStencilView(m_DSV_Depth.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->ClearRenderTargetView(m_RTV_Ambient.Get(), color);
	deviceContext->ClearDepthStencilView(m_DSV_Ambient.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->ClearRenderTargetView(m_RTV_Texture.Get(), color);
	deviceContext->ClearDepthStencilView(m_DSV_Texture.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* GBufferRT::GetShaderResourceViewNormal()
{
	return m_SRV_Normal.Get();
}
ID3D11ShaderResourceView** GBufferRT::GetShaderResourceViewAddressNormal()
{
	return m_SRV_Normal.GetAddressOf();
}

ID3D11ShaderResourceView* GBufferRT::GetShaderResourceViewDepth()
{
	return m_SRV_Depth.Get();
}
ID3D11ShaderResourceView** GBufferRT::GetShaderResourceViewAddressDepth()
{
	return m_SRV_Depth.GetAddressOf();
}

ID3D11ShaderResourceView* GBufferRT::GetShaderResourceViewAmbient()
{
	return m_SRV_Ambient.Get();
}
ID3D11ShaderResourceView** GBufferRT::GetShaderResourceViewAddressAmbient()
{
	return m_SRV_Ambient.GetAddressOf();
}

ID3D11ShaderResourceView* GBufferRT::GetShaderResourceViewTexture()
{
	return m_SRV_Texture.Get();
}
ID3D11ShaderResourceView** GBufferRT::GetShaderResourceViewAddressTexture()
{
	return m_SRV_Texture.GetAddressOf();
}

XMMATRIX GBufferRT::GetProjectionMatrix()
{
	return m_projectionMatrix;
}