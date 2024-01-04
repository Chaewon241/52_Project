#include "pch.h"
#include "Helper.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Node.h"
#include "NodeAnimation.h"
#include "Animation.h"

Node::Node()
{
	m_ParentNode = nullptr;
}

Node::~Node()
{
}

void Node::Update(float deltaTime)
{
	if (m_pNodeAnimation != nullptr)
	{
		Vector3 position, scaling;
		Quaternion rotation;

		m_pNodeAnimation->Evaluate(deltaTime, position, rotation, scaling);

		m_LocalTransform = Matrix::CreateScale(scaling) *
			Matrix::CreateFromQuaternion(rotation) *
			Matrix::CreateTranslation(position);
	}

	if (m_ParentNode != nullptr)
		m_WorldTransform = m_LocalTransform * m_ParentNode->m_WorldTransform;
	else
		m_WorldTransform = m_LocalTransform;

	for (auto& child : m_Children)
	{
		child.Update(deltaTime); 
	} 
}

void Node::CreateNode(Model* model, aiNode* node, Animation* anim)
{
	//ainode에서 matrix로 트랜스폼
	m_LocalTransform = Matrix(&node->mTransformation.a1).Transpose();
	m_Nname = node->mName.C_Str();

	// 노드에 있는 매쉬 개수
	m_MeshIndices.resize(node->mNumMeshes);

	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		UINT meshIndex = node->mMeshes[i];
		m_MeshIndices[i] = meshIndex;

		// Mesh와 Node의 WorldMatrix를 연결한다.
		model->m_Meshes[meshIndex].m_pWorldTransform = &m_WorldTransform;
	}

	for (auto& mesh : model->m_Meshes)
	{
		for (auto& boneRef : mesh.m_BoneReference)
		{
			if (boneRef.NodeName.compare(m_Nname) == 0)
			{
				boneRef.pNodeWorldMatrix = &m_WorldTransform;
			}
		}
	}

	// 애니메이션의 노드 이름과 지금 받아온 노드 이름과 같을 때를 판단
	for (auto& child : anim->m_NodeAnims)
	{
		if (child.m_NodeName.compare(m_Nname) == 0)
		{
			m_pNodeAnimation = &child;
			break;
		}
	}

	m_Children.resize(node->mNumChildren);
	
	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		m_Children[i].m_ParentNode = this;
		m_Children[i].CreateNode(model, node->mChildren[i], anim);
	}
}
