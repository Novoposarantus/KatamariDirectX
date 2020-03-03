#pragma once
#include "Graphics/AdapterReader.h"
#include "Graphics/Shaders.h"
#include "Graphics/Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Camera.h"
#include "..\\Timer.h"
#include "Graphics/Vertex.h"



class MainPlane
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device);
	void RenderFrame(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
private:
	bool InitializeShaders(Microsoft::WRL::ComPtr<ID3D11Device> device);
	bool InitializeScene(Microsoft::WRL::ComPtr<ID3D11Device> device);

	VertexShader vertexshader;
	PixelShader pixelshader;

	VertexBuffer<MainPlaneVertex> vertexBuffer;
	IndexBuffer indexBuffer;

};