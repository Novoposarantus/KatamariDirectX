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

	//if (!mainPlane.Initialize(device))
	//	return false;

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	UINT offset = 0;

	this->mainObject.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	this->mainPlane.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		 if (!this->gameObjects[i].IsAttachedToMain() && this->gameObjects[i].CheckColision(this->mainObject, this->mainObjectSize))
		{
			this->gameObjects[i].AttachToMain(&this->mainObject);
			this->mainObjectSize += this->gameObjects[i].size / 2;
		}
		this->gameObjects[i].Draw(camera.GetViewMatrix()* camera.GetProjectionMatrix());
	}

#pragma region DrawText
	static int fpsCounter = 0;
	fpsCounter += 1;
	static std::string fpsStirng = "FPS: 0";
	if (fpsTimer.GetMilisecondsElapsed() > 1000)
	{
		fpsStirng = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	spriteBatch->Begin();
	spriteFont->DrawString(
		spriteBatch.get(),
		StringHelper::StringToWide(fpsStirng).c_str(),
		DirectX::XMFLOAT2(0, 0),
		DirectX::Colors::White,
		0.0f,
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(1.0f, 1.0f)
	);
	spriteFont->DrawString(
		spriteBatch.get(), 
		L"Katamari damacy", 
		DirectX::XMFLOAT2(0, 25), 
		DirectX::Colors::White, 
		0.0f, 
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(1.0f, 1.0f)
	);

	//std::stringstream size_ss;
	//size_ss << std::fixed << std::setprecision(2) << this->mainObjectSize;

	//auto size_string = "Size: " + size_ss.str();
	//spriteFont->DrawString(
	//	spriteBatch.get(),
	//	size_string.c_str(),
	//	DirectX::XMFLOAT2(0, 50),
	//	DirectX::Colors::White,
	//	0.0f,
	//	DirectX::XMFLOAT2(0, 0),
	//	DirectX::XMFLOAT2(1.0f, 1.0f)
	//);
	//auto pos = mainObject.GetPosition();
	//auto vec_pos_string = "Position main X: " + std::to_string(pos.x) + ", Y: " + std::to_string(pos.y) + ", Z: " + std::to_string(pos.z);
	//spriteFont->DrawString(
	//	spriteBatch.get(),
	//	vec_pos_string.c_str(),
	//	DirectX::XMFLOAT2(0, 50),
	//	DirectX::Colors::White,
	//	0.0f,
	//	DirectX::XMFLOAT2(0, 0),
	//	DirectX::XMFLOAT2(1.0f, 1.0f)
	//);

	//auto rot = mainObject.GetRotation();
	//auto vec_rot_string = "Rotation main: X: " + std::to_string(rot.x) + ", Y: " + std::to_string(rot.y) + ", Z: " + std::to_string(rot.z);
	//spriteFont->DrawString(
	//	spriteBatch.get(),
	//	vec_rot_string.c_str(),
	//	DirectX::XMFLOAT2(0, 75),
	//	DirectX::Colors::White,
	//	0.0f,
	//	DirectX::XMFLOAT2(0, 0),
	//	DirectX::XMFLOAT2(1.0f, 1.0f)
	//);

	//auto forvardCam = camera.GetForwardVector();
	//auto vec_forvardCam_string = "Forvard Cam X: " + std::to_string(forvardCam.x) + ", Y: " + std::to_string(forvardCam.y) + ", Z: " + std::to_string(forvardCam.z);
	//spriteFont->DrawString(
	//	spriteBatch.get(),
	//	vec_forvardCam_string.c_str(),
	//	DirectX::XMFLOAT2(0, 100),
	//	DirectX::Colors::White,
	//	0.0f,
	//	DirectX::XMFLOAT2(0, 0),
	//	DirectX::XMFLOAT2(1.0f, 1.0f)
	//);

	//auto leftCam = camera.GetLeftVector();
	//auto vec_leftCam_string = "Left Cam X: " + std::to_string(leftCam.x) + ", Y: " + std::to_string(leftCam.y) + ", Z: " + std::to_string(leftCam.z);
	//spriteFont->DrawString(
	//	spriteBatch.get(),
	//	vec_leftCam_string.c_str(),
	//	DirectX::XMFLOAT2(0, 125),
	//	DirectX::Colors::White,
	//	0.0f,
	//	DirectX::XMFLOAT2(0, 0),
	//	DirectX::XMFLOAT2(1.0f, 1.0f)
	//);

	//auto vec_forvard_string = std::to_string(this->gameObjects[0].CheckColision(this->mainObject));
	//spriteFont->DrawString(
	//	spriteBatch.get(),
	//	vec_forvard_string.c_str(),
	//	DirectX::XMFLOAT2(0, 75),
	//	DirectX::Colors::White,
	//	0.0f,
	//	DirectX::XMFLOAT2(0, 0),
	//	DirectX::XMFLOAT2(1.0f, 1.0f)
	//);

	spriteBatch->End();
#pragma endregion


	this->swapchain->Present(1, NULL);
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
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
			NULL, //Flags for runtime layers
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
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));

		//Set the Viewport
		this->deviceContext->RSSetViewports(1, &viewport);

		//Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(
			&rasterizerDesc,
			this->rasterizerState.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//Create Rasterizer State Culing front
		/*CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		hr = this->device->CreateRasterizerState(
			&rasterizerDesc_CullFront,
			this->rasterizerState_CullFront.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");*/

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
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
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

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
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
		auto hr = cb_vs_VertexShader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = cb_ps_PixelShader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		//Initialize model(s)
		if (!mainObject.Initialize("Data\\Objects\\Samples\\orange_embeddedtexture.fbx",this->device.Get(), this->deviceContext.Get(), this->cb_vs_VertexShader))
			return false;
		float mainStartSize = 0.5f;
		mainObject.SetScale(1, 1, 1, mainStartSize);
		this->mainObjectSize = mainStartSize;

		for (int i = 0; i < 10; ++i)
		{
			GameObject gameObject;
			gameObject.Initialize(
				"Data\\Objects\\Samples\\orange_embeddedtexture.fbx", 
				this->device.Get(), 
				this->deviceContext.Get(), 
				this->cb_vs_VertexShader
			);
			float x = rand() % 200 - 100;
			float z = rand() % 200 - 100; 
			float r = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.7f - 0.2f)));
			gameObject.SetPosition(x, 0, z);
			gameObject.SetScale(1, 1, 1, r);
			gameObjects.push_back(gameObject);
		}

		const float skullScaleMod = 0.05f;
		for (int i = 0; i < 10; ++i)
		{
			GameObject gameObject;
			gameObject.Initialize(
				"Data\\Objects\\Skull\\12140_Skull_v3_L2.obj",
				this->device.Get(),
				this->deviceContext.Get(),
				this->cb_vs_VertexShader
			);
			float x = rand() % 200 - 100;
			float z = rand() % 200 - 100;
			float r = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.2f)));
			gameObject.SetPosition(x, 0, z);
			gameObject.SetRotation(90, 0, 0);
			gameObject.SetScale(skullScaleMod, skullScaleMod, skullScaleMod , r);
			gameObjects.push_back(gameObject);
		}

		const float nanusuitScaleMod = 0.09f;

		for (int i = 0; i < 10; ++i)
		{
			GameObject gameObject;
			gameObject.Initialize(
				"Data\\Objects\\nanosuit\\nanosuit.obj",
				this->device.Get(),
				this->deviceContext.Get(),
				this->cb_vs_VertexShader
			);
			float x = rand() % 200 - 100;
			float z = rand() % 200 - 100;
			float r = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - 0.2f)));
			gameObject.SetPosition(x, 0, z);
			gameObject.SetScale(nanusuitScaleMod, nanusuitScaleMod, nanusuitScaleMod, r);
			gameObjects.push_back(gameObject);
		}

		const float alocasiaScaleMod = 0.001f;

		for (int i = 0; i < 1; ++i)
		{
			GameObject gameObject;
			gameObject.Initialize(
				"Data\\Objects\\Alocasia\\01Alocasia_obj.obj",
				this->device.Get(),
				this->deviceContext.Get(),
				this->cb_vs_VertexShader
			);
			float x = rand() % 200 - 100;
			float z = rand() % 200 - 100;
			float r = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f - 0.2f)));
			gameObject.SetPosition(x, 0, z);
			gameObject.SetScale(alocasiaScaleMod, alocasiaScaleMod, alocasiaScaleMod, r);
			gameObjects.push_back(gameObject);
		}

		//Initialize model(s)
		if (!mainPlane.Initialize(this->device.Get(), this->deviceContext.Get(), this->cb_vs_VertexShader))
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
