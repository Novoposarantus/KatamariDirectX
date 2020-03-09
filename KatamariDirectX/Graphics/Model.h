#pragma once
#include "Mesh.h"

using namespace DirectX;

class Model
{
public:
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext, 
		ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader
	);
	void Draw(const DirectX::SimpleMath::Matrix& worldMatrix, const DirectX::SimpleMath::Matrix& viewProjectionMatrix);
	const DirectX::SimpleMath::Vector3& GetMinDirections();
	const DirectX::SimpleMath::Vector3& GetMaxDirections();


private:
	std::vector<Mesh> meshes;
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::SimpleMath::Matrix& parentTransformMatrix);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::SimpleMath::Matrix& transformMatrix);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	int GetTextureIndex(aiString* pStr);


	float xPlus = 0;
	float yPlus = 0;
	float zPlus = 0;
	float xMinus = 0;
	float yMinus = 0;
	float zMinus = 0;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_VertexShader>* cb_vs_vertexshader = nullptr;
	std::string directory = "";
};