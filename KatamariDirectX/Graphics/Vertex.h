#pragma once
#include <d3d11.h>
#include <SimpleMath.h>


struct Vertex
{
	Vertex() = default;
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) 
		: 
		pos(x, y, z),
		texCoord(u, v),
		normal(nx, ny, nz)
	{}
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector2 texCoord;
	DirectX::SimpleMath::Vector3 normal;
};