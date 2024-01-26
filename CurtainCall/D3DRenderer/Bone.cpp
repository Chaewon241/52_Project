#include "pch.h"
#include "Bone.h"
#include "ModelResource.h"
#include "NodeAnimation.h"
// #include "TutorialApp.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>

#include "SkeletalMeshModel.h"

namespace Math = DirectX::SimpleMath;

void Bone::Create(ModelResource* model, aiNode* node, Animation* anim)
{
	m_localTransform = Math::Matrix(&node->mTransformation.a1).Transpose();
	m_name = node->mName.C_Str();

	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		UINT meshIndex = node->mMeshes[i];

		// Mesh와 Node의 WorldMatrix를 연결한다.
		 model->m_Meshes[meshIndex].m_pNodeWorld = &m_worldTransform; 
	}

	for (auto& mesh : model->m_Meshes)
	{
		for (auto& boneRef : mesh.m_BoneReferences)
		{
			if (boneRef.NodeName.compare(m_name) == 0)
			{
				boneRef.NodeWorldMatrixPtr = &m_worldTransform;
			}
		}
	}

	for (auto& child : anim->m_NodeAnims)
	{
		if (child.m_NodeName.compare(m_name) == 0) // if  equal
		{
			m_pNodeAnimation = &child;
			break;
		}
	}

	m_Children.resize(node->mNumChildren);
	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		m_Children[i].m_pParentBone = this;
		m_Children[i].Create(model, node->mChildren[i], anim);
	}
}

void Bone::Update(float deltaTime)
{
	if (m_pNodeAnimation != nullptr)
	{
		Math::Vector3 position, scaling;
		Math::Quaternion rotation;

		m_pNodeAnimation->Evaluate(*m_pAnimationTime, position, rotation, scaling);

		m_localTransform = Math::Matrix::CreateScale(scaling) *
			Math::Matrix::CreateFromQuaternion(rotation) *
			Math::Matrix::CreateTranslation(position);
	}	

	if (m_pParentBone != nullptr)
		m_worldTransform = m_localTransform * m_pParentBone->m_worldTransform;
	else
		m_worldTransform = m_localTransform * *m_pParentModel->m_worldMatrix;

	for (auto& child : m_Children)
	{
		child.Update(deltaTime);
	}
}

Bone* Bone::FindNode(const std::string& name)
{
	if (m_name == name)
		return this;

	for (auto& child : m_Children)
	{
		Bone* found = child.FindNode(name);
		if (found != nullptr)
			return found;
	}

	return nullptr;
}

Bone& Bone::CreateChild()
{
	return m_Children.emplace_back();
}

