#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Camera.h"
#include "..\\Timer.h"
#include "./Meshes/MainPlane.h"
#include "RenderableGameObject.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "Light.h"
#include "RenderTarget.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void RenderToRexture();
	void RenderToWindow();
	Camera camera;
	std::vector<RenderableGameObject> gameObjects;
	RenderableGameObject mainObject;
	float mainObjectSize = 1;
	MainPlane mainPlane;
	Light light;

private:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexshader;
	PixelShader pixelshader;
	ConstantBuffer<CB_VS_VertexShader> cb_vs_VertexShader;
	ConstantBuffer<CB_PS_Light> cb_ps_Light;

	VertexShader depthVertexShader;
	PixelShader depthPixelShader;
	ConstantBuffer<CB_VS_DEPTH> cb_ps_depth;

	RenderTarget* renderTexture;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	CD3D11_VIEWPORT viewport;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	//Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;


	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> depthSamplerState;

	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
};