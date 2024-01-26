#pragma once
#include "RenderComponent.h"

class SkeletalMeshModel;

class SkeletalMeshComponent
	: public RenderComponent
{
	// update ���� ó���� �������� �Űܾ� ��. �ִϸ��̼� ����, bone Update
	// Bone�� �ִ� ��Ʈ���� �����Ű��
	// playAnimation ������Ʈ���� �����ϵ��� �ϱ�

public:
	SkeletalMeshComponent() = default;
	~SkeletalMeshComponent() override;

public:
	std::shared_ptr<SkeletalMeshModel> GetSkeletalMeshModel() const { return m_skeletalMeshModel; }

	// CreateComponent �� �ʼ������� filePath ����
	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }
	void SetCurAnimation(std::string name);
	void SetPlaySpeed(float speed) { m_animationPlaySpeed = speed; }

	void AddAnimation(std::string filePath);

public:
	void Initialize() override;	// SkeletalMeshModel�� �������� ���� ����, �޾ƿ´�.
	void Update(float deltaTime) override;


private:
	std::string m_filePath;
	std::shared_ptr<SkeletalMeshModel> m_skeletalMeshModel;

	int  m_animationIndex = 0;
	float m_animationProgressTime = 0.0f;
	float m_animationPlaySpeed = 1.f;

	std::unordered_map<std::string, int> m_animationMap;
};

