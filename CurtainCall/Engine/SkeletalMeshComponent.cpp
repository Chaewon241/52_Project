#include "pch.h"
#include "SkeletalMeshComponent.h"

#include "../D3DRenderer/SkeletalMeshModel.h"

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	m_skeletalMeshModel.reset();
	m_animationMap.clear();
}

void SkeletalMeshComponent::SetCurAnimation(std::string name)
{
	m_animationIndex = m_animationMap.find(name)->second;
	m_skeletalMeshModel->PlayAnimation(m_animationIndex);
}

void SkeletalMeshComponent::AddAnimation(std::string filePath)
{
	m_skeletalMeshModel->AddSceneAnimationFromFBX(filePath);
	m_animationMap.emplace(filePath, m_animationMap.size());
}

void SkeletalMeshComponent::Initialize()
{
	assert(!m_filePath.empty());

	m_skeletalMeshModel = std::make_shared<SkeletalMeshModel>();
	m_skeletalMeshModel->SetWorldMatrix(&m_worldMatrix);
	m_skeletalMeshModel->SetAnimationTime(&m_animationProgressTime);
	m_skeletalMeshModel->ReadSceneResourceFromFBX(m_filePath);
	m_animationMap.emplace(m_filePath, m_animationMap.size());

	m_skeletalMeshModel->PlayAnimation(m_animationIndex);

	RenderComponent::Initialize();
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	RenderComponent::Update(deltaTime);

	if (!m_animationMap.empty())
	{
		m_animationProgressTime += deltaTime * m_animationPlaySpeed;
		m_animationProgressTime = (float)fmod(m_animationProgressTime,  m_skeletalMeshModel->GetAnimationDuration(m_animationIndex));
	}

	m_skeletalMeshModel->UpdateBoneNode(deltaTime);
}