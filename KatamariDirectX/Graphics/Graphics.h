#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Camera.h"
#include "..\\Timer.h"
#include "./Meshes/MainPlane.h"
#include "Model.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera camera;
	Model model;
	MainPlane mainPlane;

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
	ConstantBuffer<CB_PS_PixelShader> cb_ps_PixelShader;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;


	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dotaTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> gradientTexture;

	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
};