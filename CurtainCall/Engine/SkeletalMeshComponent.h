#pragma once
#include "RenderComponent.h"

class SkeletalMeshModel;

class SkeletalMeshComponent
	: public RenderComponent
{
	// update 관련 처리들 엔진으로 옮겨야 함. 애니메이션 진행, bone Update
	// Bone에 있는 매트릭스 연결시키기
	// playAnimation 컴포넌트에서 조작하도록 하기

public:
	SkeletalMeshComponent() = default;
	~SkeletalMeshComponent() override;

public:
	std::shared_ptr<SkeletalMeshModel> GetSkeletalMeshModel() const { return m_skeletalMeshModel; }

	// CreateComponent 후 필수적으로 filePath 설정
	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }
	void SetCurAnimation(std::string name);
	void SetPlaySpeed(float speed) { m_animationPlaySpeed = speed; }

	void AddAnimation(std::string filePath);

public:
	void Initialize() override;	// SkeletalMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;


private:
	std::string m_filePath;
	std::shared_ptr<SkeletalMeshModel> m_skeletalMeshModel;

	int  m_animationIndex = 0;
	float m_animationProgressTime = 0.0f;
	float m_animationPlaySpeed = 1.f;

	std::unordered_map<std::string, int> m_animationMap;
};

