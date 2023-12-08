#include "pch.h"
#include "Material.h"
#include "Helper.h"

#include <filesystem>
#include <assimp/scene.h>
#include <Directxtk/WICTextureLoader.h>

Material::Material()
{
}

Material::~Material()
{
}

void Material::CreateMaterial(ID3D11Device* device, aiMaterial* pMaterial)
{
	aiString texturePath;
	wstring basePath = L"../Resource/";
	filesystem::path path;
	wstring finalPath;
	string name = pMaterial->GetName().C_Str();


	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_pTextureRV));
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_pNormalRV));
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_pSpecularRV));
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device, finalPath.c_str(), &m_pEmissiveRV));
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device, finalPath.c_str(), &m_pOpacityRV));
	}
}

