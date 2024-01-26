#pragma once
#include "RenderComponent.h"

class StaticMeshModel;

class StaticMeshComponent
	: public RenderComponent
{
public:
	StaticMeshComponent() = default;
	~StaticMeshComponent() override;

public:
	std::shared_ptr<StaticMeshModel> GetStaticMeshModel() const { return m_staticMeshModel; }

	// CreateComponent 후 필수적으로 filePath 설정
	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }

public:
	void Initialize() override;	// staticMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;


private:
	std::string m_filePath;
	std::shared_ptr<StaticMeshModel> m_staticMeshModel;
};

