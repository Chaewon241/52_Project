#include "pch.h"
#include "Model.h"
#include "Helper.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::Initialize()
{
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	importFlags =
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights |
		aiProcess_ConvertToLeftHanded;
}

void Model::Update(float deltaTime)
{
	m_RootNode.Update(deltaTime);
}

bool Model::CreateModel(ID3D11Device* device, const char* filePath)
{

	m_Model = importer.ReadFile(filePath, importFlags);

	if (!m_Model)
	{
		return false;
	}

	m_Meshes.resize(m_Model->mNumMeshes);
	m_Materials.resize(m_Model->mNumMaterials);

	for (unsigned int i = 0; i < m_Model->mNumMaterials; ++i)
	{
		m_Materials[i].CreateMaterial(device, m_Model->mMaterials[i]);
	}

	for (int i = 0; i < m_Model->mNumAnimations; i++)
	{
		if (m_Model->mAnimations[i] != nullptr)
			m_Animation.Create(m_Model->mAnimations[i]);
	}

	for (unsigned int i = 0; i < m_Model->mNumMeshes; ++i)
	{
		m_Meshes[i].CreateMesh(device, m_Model->mMeshes[i]);
	}

	m_RootNode.CreateNode(this, m_Model->mRootNode, &m_Animation);

	importer.FreeScene();
}

