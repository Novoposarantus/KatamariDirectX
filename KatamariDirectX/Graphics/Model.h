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
		bool gamma
	);
	void Draw(ConstantBuffer<CB_VS_Mesh_Transform>& cb_vs_vertexshader);
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


	float xPlus;
	float yPlus;
	float zPlus;
	float xMinus;
	float yMinus;
	float zMinus;
	bool gamma;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	std::string directory = "";
};