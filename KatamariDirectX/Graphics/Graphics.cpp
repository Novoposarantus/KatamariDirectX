#include "Graphics.h"
#include "..\\Macros.h"
#include <algorithm>
#include <cstdlib>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <sstream>
#include <iomanip>

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	fpsTimer.Start();
	this->windowWidth = width;
	this->windowHeight = height;

	if (!InitializeDirectX(hwnd, width, height))
		return false;


	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	if (!InitializeDirect2D(hwnd, width, height))
		return false;

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	try
	{

		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		std::sort(adapters.begin(), adapters.end(),
			[](const AdapterData& a, const AdapterData& b)
			{
				return a.description.DedicatedVideoMemory > b.description.DedicatedVideoMemory;
			});

		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scd = { 0 };

		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = gammaCorrection ? DXGI_FORMAT_B8G8R8A8_UNORM_SRGB : DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(
			adapters[0].pAdapter, //IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, // For software driver type
			D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, //Flags for runtime layers
			NULL, //feature levels array
			0, //# of feature levels in array
			D3D11_SDK_VERSION,
			&scd, //Swapchain description
			this->swapchain.GetAddressOf(), // Swapchain Address
			this->device.GetAddressOf(), // Device Address
			NULL, // supported feature level,
			this->deviceContext.GetAddressOf() // Device Context Address
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		//Describe our Depth/Stencil Buffer
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			this->windowWidth,
			this->windowHeight
		);
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		//Create depth stencil state
		CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		//Create the Viewport

		viewport.Width = static_cast<float>(this->windowWidth);
		viewport.Height = static_cast<float>(this->windowHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		//Set the Viewport
		this->deviceContext->RSSetViewports(1, &viewport);

		renderTarget = new RenderTarget();
		if (!renderTarget->Init(this->device.Get(), 0.1f, 10000.0f))
			return false;


		renderTargetHDR = new RenderTarget();
		if (!renderTargetHDR->Init(this->device.Get(), 0.1f, 10000.0f))
			return false;


		//Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(
			&rasterizerDesc,
			this->rasterizerState.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//Create Blend State
		D3D11_BLEND_DESC blendDesc = { 0 };

		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };

		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbd;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		//Create sampler description for sampler state
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = 0.0f;
		sampDesc.BorderColor[1] = 0.0f;
		sampDesc.BorderColor[2] = 0.0f;
		sampDesc.BorderColor[3] = 0.0f;
		sampDesc.MinLOD = 0;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&sampDesc, this->wrapSamplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		hr = this->device->CreateSamplerState(&sampDesc, this->clampSamplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create depth sampler state.");

	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders()
{
	ShaderFolder();

	D3D11_INPUT_ELEMENT_DESC layoutDepthDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SPEC_COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numDepthElements = ARRAYSIZE(layoutDepthDesc);

	if (!depthVertexshader.Initialize(
		this->device,
		shaderFolder + L"DepthVertexShader.cso",
		layoutDepthDesc,
		numDepthElements))
	{
		return false;
	}

	if (!depthPixelshader.Initialize(this->device, shaderFolder + L"DepthPixelShader.cso"))
	{
		return false;
	}


	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SPEC_COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layoutDesc);

	if (!vertexshader.Initialize(
		this->device, 
		shaderFolder + L"VertexShader.cso",
		layoutDesc,
		numElements))
	{
		return false;
	}

	if (!pixelshader.Initialize(this->device, shaderFolder + L"PixelShader.cso"))
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeScene()
{
	try {

		camera.SetParent(&this->mainObject);
		//Initialize Constant Buffer

		auto hr = cb_vs_cam.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = cb_ps_Light.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");


		hr = cb_vs_depth.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");


		hr = cb_vs_mesh_transform.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		//Initialize model(s)
		if (!mainObject.Initialize("Data\\Objects\\Samples\\orange_embeddedtexture.fbx",this->device.Get(), this->deviceContext.Get(), this->gammaCorrection))
			return false;

		if (!directionalLight.Initialize(0.5f, 100.0f))
			return false;

		this->cb_ps_Light.data.directionalLightColor = directionalLight.GetDirectionalColor();
		this->cb_ps_Light.data.directionalLightStrenght = directionalLight.GetDirectionalStrength();
		this->cb_ps_Light.data.directionalLightDir = directionalLight.GetDirection();

		const float orangeScale = 1.8f;
		float mainStartSize = 0.5f;
		mainObject.SetScale(orangeScale, orangeScale, orangeScale);
		mainObject.SetSize(mainStartSize);
		mainObject.SetPosition(0, -0.1, 0);
		this->mainObjectSize = mainStartSize;

		for (int i = 0; i < 20; ++i)
		{
			RenderableGameObject gameObject;
			gameObject.Initialize(
				"Data\\Objects\\Samples\\orange_embeddedtexture.fbx", 
				this->device.Get(), 
				this->deviceContext.Get(),
				this->gammaCorrection
			);
			float x = rand() % 200 - 100;
			float z = rand() % 200 - 100; 
			float r = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.7f - 0.2f)));
			gameObject.SetPosition(x, 0, z);
			gameObject.SetScale(orangeScale, orangeScale, orangeScale);
			gameObject.SetSize(r);
			gameObjects.push_back(gameObject);
		}

		//Initialize model(s)
		if (!mainPlane.Initialize(this->device.Get(), this->deviceContext.Get()))
			return false;


		camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 10000.0f);
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

void Graphics::RenderFrame()
{
	this->directionalLight.UpdateViewMatrix(this->mainObject.GetPosition());

	this->cb_ps_Light.data.camPos = camera.GetPosition();
	this->cb_ps_Light.data.specPower = this->directionalLight.specPower;
	this->cb_ps_Light.ApplyChanges();
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_Light.GetAddressOf());

	this->RenderToTexture();
	this->RenderToHDRTexture();
	this->RenderToWindow();
}

void Graphics::RenderToWindow()
{

	// Сбрасываем render target (теперь снова будет рисовать на экран)
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());
	// Сбрасываем вьюпорт
	this->deviceContext->RSSetViewports(1, &viewport);

	float bgcolor[] = { 0.0, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);

	this->deviceContext->PSSetShaderResources(1, 1, renderTarget->GetShaderResourceViewAddress());
	this->deviceContext->GenerateMips(this->renderTargetHDR->GetShaderResourceView());
	this->deviceContext->PSSetShaderResources(2, 1, renderTargetHDR->GetShaderResourceViewAddress());
	this->deviceContext->PSSetSamplers(0, 1, this->wrapSamplerState.GetAddressOf());
	this->deviceContext->PSSetSamplers(1, 1, this->clampSamplerState.GetAddressOf());


	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	this->deviceContext->VSSetConstantBuffers(1, 1, this->cb_vs_cam.GetAddressOf());
	this->cb_vs_cam.data.camProjMatrix = camera.GetProjectionMatrix();
	this->cb_vs_cam.data.camViewMatrix = camera.GetViewMatrix();
	this->cb_vs_cam.data.camShadowProjMatrix = directionalLight.GetProjectionMatrix();
	this->cb_vs_cam.data.camShadowViewMatrix = directionalLight.GetViewMatrix();
	auto camPos = camera.GetPosition();
	this->cb_vs_cam.data.camPos = Vector4(camPos.x, camPos.y, camPos.z, 1.0f);
	this->cb_vs_cam.ApplyChanges();

	this->mainPlane.Draw(this->cb_vs_mesh_transform);
	this->mainObject.Draw(this->cb_vs_mesh_transform);
	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		if (!this->gameObjects[i].IsAttachedToMain()
			&& this->gameObjects[i].CanAttach(this->mainObjectSize)
			&& this->gameObjects[i].CheckColision(this->mainObject))
		{
			this->gameObjects[i].AttachToMain(&this->mainObject);
			this->mainObjectSize += this->gameObjects[i].GetSize() / 2;
			this->attached++;
		}
		this->gameObjects[i].Draw(this->cb_vs_mesh_transform);
	}

	static int fpsCounter = 0;
	fpsCounter += 1;
	static std::string fpsStirng = "FPS: ";
	if (fpsTimer.GetMilisecondsElapsed() > 1000)
	{
		fpsStirng = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}

	static std::string attachedCounterString = "";
	attachedCounterString = "Attached: " + std::to_string(this->attached);

	this->renderTarget2D->BeginDraw();
	auto rec1 = D2D1::RectF(0.0f, 0, 1280, 800);
	auto rec2 = D2D1::RectF(0.0f, 42, 1280, 800);
	pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
	this->renderTarget2D->DrawTextW(StringHelper::StringToWide(fpsStirng).c_str(), fpsStirng.length(), pDTextFormat.Get(), &rec1, pSolidBrush.Get());
	this->renderTarget2D->DrawTextW(StringHelper::StringToWide(attachedCounterString).c_str(), attachedCounterString.length(), pDTextFormat.Get(), &rec2, pSolidBrush.Get());
	this->renderTarget2D->EndDraw();

#pragma region ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto camPosString = std::to_string(camPos.x) + "," + std::to_string(camPos.y) + "," + std::to_string(camPos.z);
	ImGui::Begin("Light Controls");
	ImGui::Text(camPosString.c_str());
	//ImGui::Image(renderTargetHDR->GetShaderResourceView(), ImVec2(400, 400));
	ImGui::DragFloat("SpecPower", &this->directionalLight.specPower, 1.0f, 1.0f, 100.0f);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


#pragma endregion

	this->deviceContext->PSSetShaderResources(3, 1, renderTargetHDR->GetShaderResourceViewAddress());
	this->swapchain->Present(1, NULL);
}

void Graphics::RenderToTexture()
{
	//Указываем что нужно рендерить в текстуру
	renderTarget->SetRenderTarget(this->deviceContext.Get());
	// Очищаем ее
	renderTarget->ClearRenderTarget(this->deviceContext.Get(), 0.0f, 0.0f, 0.0f, 1.0f);

	this->deviceContext->IASetInputLayout(this->depthVertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	this->deviceContext->VSSetShader(depthVertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(depthPixelshader.GetShader(), NULL, 0);


	this->deviceContext->VSSetConstantBuffers(1, 1, this->cb_vs_depth.GetAddressOf());
	this->cb_vs_depth.data.camShadowProjMatrix = this->directionalLight.GetProjectionMatrix();
	this->cb_vs_depth.data.camShadowViewMatrix = this->directionalLight.GetViewMatrix();
	this->cb_vs_depth.ApplyChanges();


	this->mainPlane.Draw(this->cb_vs_mesh_transform);
	this->mainObject.Draw(this->cb_vs_mesh_transform);
	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		this->gameObjects[i].Draw(this->cb_vs_mesh_transform);
	}

}

bool Graphics::InitializeDirect2D(HWND hwnd, int width, int height) 
{
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, this->pD2D1Factory.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "D2D1CreateFactory");

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(this->pDWriteFactory.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "DWriteCreateFactory");

	ID3D11Resource* res;
	this->renderTargetView->GetResource(&res);

	IDXGISurface* surface;
	hr = res->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(&surface));
	COM_ERROR_IF_FAILED(hr, "QueryInterface");

	hr = this->pD2D1Factory->CreateDxgiSurfaceRenderTarget(
		surface,
		D2D1_RENDER_TARGET_PROPERTIES{
			D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1_PIXEL_FORMAT {
				DXGI_FORMAT_UNKNOWN,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			},
			static_cast<FLOAT>(96),
			static_cast<FLOAT>(96)
		},
		this->renderTarget2D.GetAddressOf()
	);
	COM_ERROR_IF_FAILED(hr, "CreateDxgiSurfaceRenderTarget");
	res->Release();
	surface->Release();

	hr = this->renderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), this->pSolidBrush.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "CreateSolidColorBrush");

	hr = pDWriteFactory->CreateTextFormat(
		L"arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"en-us",
		pDTextFormat.GetAddressOf()
	);
	COM_ERROR_IF_FAILED(hr, "CreateTextFormat");

	return true;
}

void Graphics::RenderToHDRTexture()
{
	//Указываем что нужно рендерить в текстуру
	renderTargetHDR->SetRenderTarget(this->deviceContext.Get());
	// Очищаем ее
	renderTargetHDR->ClearRenderTarget(this->deviceContext.Get(), 0.0f, 0.0f, 0.0f, 1.0f);

	float bgcolor[] = { 0.0, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);

	this->deviceContext->PSSetShaderResources(1, 1, renderTarget->GetShaderResourceViewAddress());
	this->deviceContext->PSSetSamplers(0, 1, this->wrapSamplerState.GetAddressOf());
	this->deviceContext->PSSetSamplers(1, 1, this->clampSamplerState.GetAddressOf());


	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	this->deviceContext->VSSetConstantBuffers(1, 1, this->cb_vs_cam.GetAddressOf());
	this->cb_vs_cam.data.camProjMatrix = camera.GetProjectionMatrix();
	this->cb_vs_cam.data.camViewMatrix = camera.GetViewMatrix();
	this->cb_vs_cam.data.camShadowProjMatrix = directionalLight.GetProjectionMatrix();
	this->cb_vs_cam.data.camShadowViewMatrix = directionalLight.GetViewMatrix();
	auto camPos = camera.GetPosition();
	this->cb_vs_cam.data.camPos = Vector4(camPos.x, camPos.y, camPos.z, 1.0f);
	this->cb_vs_cam.ApplyChanges();

	this->mainPlane.Draw(this->cb_vs_mesh_transform);
	this->mainObject.Draw(this->cb_vs_mesh_transform);
	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		this->gameObjects[i].Draw(this->cb_vs_mesh_transform);
	}
}
