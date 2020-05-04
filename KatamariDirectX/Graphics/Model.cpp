#include "Model.h"

bool Model::Initialize(
	const std::string& filePath,
	ID3D11Device* device, 
	ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;

	try
	{
		if (!this->LoadModel(filePath))
			return false;

	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}


void Model::Draw(
	ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader,
	const DirectX::SimpleMath::Matrix& worldMatrix, 
	const DirectX::SimpleMath::Matrix& viewProjectionMatrix,
	const DirectX::SimpleMath::Matrix& viewProjectionMatrixLight)
{
	CB_VS_VertexShader cb;

	this->deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader.GetAddressOf());

	for (int i = 0; i < meshes.size(); i++)
	{
		//Update Constant buffer with WVP Matrix
		cb_vs_vertexshader.data.wvpMatrix = meshes[i].GetTransformMatrix() *  worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
		cb_vs_vertexshader.data.worldMatrix = meshes[i].GetTransformMatrix() * worldMatrix; //Calculate World-Projection Matrix
		cb_vs_vertexshader.data.wvpLight = meshes[i].GetTransformMatrix() * viewProjectionMatrixLight;
		cb_vs_vertexshader.ApplyChanges();
		meshes[i].Draw();
	}
}

void Model::Draw(
	ConstantBuffer<CB_VS_DEPTH>& cb_vs_vertexshader,
	const DirectX::SimpleMath::Matrix& worldMatrix,
	const DirectX::SimpleMath::Matrix& viewProjectionMatrix)
{
	CB_VS_DEPTH cb;

	this->deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader.GetAddressOf());

	for (int i = 0; i < meshes.size(); i++)
	{
		cb_vs_vertexshader.data.WVP = meshes[i].GetTransformMatrix() * worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
		cb_vs_vertexshader.ApplyChanges();
		meshes[i].Draw();
	}
}

const DirectX::SimpleMath::Vector3& Model::GetMinDirections()
{
	return DirectX::SimpleMath::Vector3(this->xMinus, this->yMinus, this->zMinus);
}

const DirectX::SimpleMath::Vector3& Model::GetMaxDirections()
{
	return DirectX::SimpleMath::Vector3(this->xPlus, this->yPlus, this->zPlus);
}

bool Model::LoadModel(const std::string& filePath)
{
	this->directory = StringHelper::GetDirectoryFromPath(filePath);
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	this->xPlus = -std::numeric_limits<float>::max();
	this->xMinus = std::numeric_limits<float>::max();
	this->yPlus = -std::numeric_limits<float>::max();
	this->yMinus = std::numeric_limits<float>::max();
	this->zPlus = -std::numeric_limits<float>::max();
	this->zMinus = std::numeric_limits<float>::max();

	this->ProcessNode(pScene->mRootNode, pScene, DirectX::SimpleMath::Matrix::Identity);
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const DirectX::SimpleMath::Matrix& parentTransformMatrix)
{
	DirectX::SimpleMath::Matrix transformMatrix = DirectX::SimpleMath::Matrix(&node->mTransformation.a1).Transpose()
		* parentTransformMatrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene, transformMatrix));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, transformMatrix);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::SimpleMath::Matrix& transformMatrix)
{
	// Data to fill
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		DirectX::SimpleMath::Vector3 vertexPos = DirectX::SimpleMath::Vector3::Transform(vertex.pos, transformMatrix);

		if (this->xPlus < vertexPos.x)
		{
			this->xPlus = vertexPos.x;
		}

		if (this->xMinus > vertexPos.x)
		{
			this->xMinus = vertexPos.x;
		}

		if (this->yPlus < vertexPos.y)
		{
			this->yPlus = vertexPos.y;
		}

		if (this->yMinus > vertexPos.y)
		{
			this->yMinus = vertexPos.y;
		}

		if (this->zPlus < vertexPos.z)
		{
			this->zPlus = vertexPos.z;
		}

		if (this->zMinus > vertexPos.z)
		{
			this->zMinus = vertexPos.z;
		}

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(this->device, this->deviceContext, vertices, indices, textures, transformMatrix);
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType)
{
	if (pMat->GetTextureCount(textureType) == 0)
		return TextureStorageType::None;

	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();
	//Check if texture is an embedded indexed texture by seeing if the file path is an index #
	if (texturePath[0] == '*')
	{
		if (pScene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	//Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}
	//Lastly check if texture is a filepath by checking for period before extension name
	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None; // No texture exists
}


std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);

	if (textureCount == 0) //If there are no textures
	{
		storetype = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) //If color = black, just use grey
			{
				materialTextures.push_back(Texture(this->device, Colors::UnloadedTextureColor, textureType));
				return materialTextures;
			}
			materialTextures.push_back(Texture(this->device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
			return materialTextures;
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType storeType = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			if (storeType == TextureStorageType::EmbeddedIndexCompressed)
			{
				int index = GetTextureIndex(&path);
				Texture embeddedIndexedTexture(
					this->device,
					reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
					pScene->mTextures[index]->mWidth,
					textureType);
				materialTextures.push_back(embeddedIndexedTexture);
				break;
			}
			else if (storeType == TextureStorageType::EmbeddedCompressed)
			{
				const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				Texture embeddedTexture(
					this->device,
					reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth,
					textureType);
				materialTextures.push_back(embeddedTexture);
			} 
			else if (storeType == TextureStorageType::Disk)
			{
				std::string filename = this->directory + '\\' + path.C_Str();
				Texture diskTexture(this->device, filename, textureType);
				materialTextures.push_back(diskTexture);
			}
		}
	}

	if (materialTextures.size() == 0)
	{
		materialTextures.push_back(Texture(this->device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
	}
	return materialTextures;
}

int Model::GetTextureIndex(aiString* pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}
