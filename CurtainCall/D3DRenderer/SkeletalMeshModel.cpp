#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "SkeletalMeshModel.h"
#include "SkeletalMeshInstance.h"
#include "ModelResource.h"

SkeletalMeshModel::SkeletalMeshModel()
{
	//D3DRenderManager::Instance->m_SkeletalMeshComponents.push_back(this);
	//m_iterator = --D3DRenderManager::Instance->m_SkeletalMeshComponents.end();
}

SkeletalMeshModel::~SkeletalMeshModel()
{
//	D3DRenderManager::Instance->m_SkeletalMeshComponents.erase(m_iterator);
}

//void SkeletalMeshModel::Update(float DeltaTime)
//{
//	//__super::Update(DeltaTime);
//
//	if (!m_ModelResource->m_Animations.empty())
//	{
//		m_AnimationProgressTime += DeltaTime;
//		m_AnimationProgressTime = (float)fmod(m_AnimationProgressTime, m_ModelResource->m_Animations[m_AnimationIndex]->m_Duration);
//	}
//
//	m_RootNode.Update(DeltaTime);
//}

bool SkeletalMeshModel::ReadSceneResourceFromFBX(std::string filePath)
{
	//m_SceneFilePath = filePath;

	 //FBX 파일 읽기
	std::shared_ptr<ModelResource> sceneResource = ResourceManager::GetInstance()->CreateModelResource(filePath, ModelType::SKELETAL);
	if (sceneResource == nullptr)
	{
		return false;
	}
	SetSceneResource(sceneResource);
	return true;
}

void SkeletalMeshModel::SetSceneResource(std::shared_ptr<ModelResource> val)
{
	assert(val);
	m_ModelResource = val;

	CreateHierachy(&m_ModelResource->m_Skeleton);

	// 인스턴스 생성
	m_MeshInstances.resize(m_ModelResource->m_Meshes.size());
	for (UINT i = 0; i < m_ModelResource->m_Meshes.size(); i++)
	{
		Mesh* meshResource = &m_ModelResource->m_Meshes[i];
		Material* material = &m_ModelResource->m_Materials[i];
		m_MeshInstances[i].Create(meshResource, &m_RootNode, material); 
	}
	UpdateBoneAnimationReference(0);	// 각 노드의 애니메이션 정보참조 연결	
	
	//	m_BoundingBox.Center = Math::Vector3(m_ModelResource->m_AABBmin + m_ModelResource->m_AABBmax) * 0.5f;	// Calculate extent
	//	m_BoundingBox.Extents = Math::Vector3(m_ModelResource->m_AABBmax - m_ModelResource->m_AABBmin);	// Calculate extent
}

void SkeletalMeshModel::UpdateBoneAnimationReference(UINT index)
{
	assert(index < m_ModelResource->m_Animations.size());
	auto animation = m_ModelResource->m_Animations[index];
	for (size_t i = 0; i < animation->m_NodeAnims.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation->m_NodeAnims[i];
		Bone* pBone = m_RootNode.FindNode(nodeAnimation.m_NodeName);
		assert(pBone != nullptr);
		pBone->m_pNodeAnimation = &animation->m_NodeAnims[i];
	}
}

void SkeletalMeshModel::PlayAnimation(UINT index)
{
	assert(index < m_ModelResource->m_Animations.size());
	UpdateBoneAnimationReference(index);
}

void SkeletalMeshModel::UpdateBoneNode(float deltaTime)
{
	m_RootNode.Update(deltaTime);
}

void SkeletalMeshModel::CreateHierachy(SkeletonResource* skeleton)
{
	UINT count = skeleton->GetBoneCount();

	BoneInfo* pRootBone = skeleton->GetBone(0);
	m_RootNode.m_name = pRootBone->Name;
	m_RootNode.m_Children.reserve(pRootBone->NumChildren);
	m_RootNode.SetParentMoel(this);

	// 0번 루트는 컨테이너이므로 현재 Node와 같다 그러므로 1번부터 시작한다.
	for (UINT i = 1; i < count; i++)
	{
		BoneInfo* pBoneInfo = skeleton->GetBone(i);
		assert(pBoneInfo != nullptr);
		assert(pBoneInfo->ParentBoneIndex != -1);

		Bone* pParentBone = m_RootNode.FindNode(skeleton->GetBoneName(pBoneInfo->ParentBoneIndex));
		assert(pParentBone != nullptr);

		auto& ChildBone = pParentBone->CreateChild();
		ChildBone.m_name = pBoneInfo->Name;
		ChildBone.m_localTransform = pBoneInfo->RelativeTransform;
		ChildBone.m_Children.reserve(pBoneInfo->NumChildren);
		ChildBone.m_pParentBone = pParentBone;
		ChildBone.m_pAnimationTime = m_pAnimationProgressTime;
	}
}

bool SkeletalMeshModel::AddSceneAnimationFromFBX(std::string filePath)
{
	assert(m_ModelResource);
	auto animation = ResourceManager::GetInstance()->CreateAnimationResource(filePath);
	if (!animation) {
		return false;
	}

	//m_AnimationFilePath.push_back(filePath);
	m_ModelResource->m_Animations.push_back(animation);
	return true;
}
Material* SkeletalMeshModel::GetMaterial(UINT index)
{
	assert(index < m_ModelResource->m_Materials.size());
	return &m_ModelResource->m_Materials[index];
}

float SkeletalMeshModel::GetAnimationDuration(int animationIdx)
{
	return m_ModelResource->m_Animations[animationIdx]->m_Duration;
}
