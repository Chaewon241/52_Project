#pragma once
#include <vector>

class SceneComponent
{
private:
public:
	std::vector<SceneComponent>* m_Children;
	SceneComponent* m_pParentScene;
	D2D_VECTOR_2F m_RelativeScale;
	D2D_VECTOR_2F m_RelativeTranslation;
	float m_RelativeRotation;
	float m_RelativeLocation;
	D2D_MATRIX_3X2_F m_RelativeTransform;
	D2D_MATRIX_3X2_F m_WorldTransform;

public:
	SceneComponent() {};
	~SceneComponent() {};

	void Init();
	void Update();
	void Render();
	void UpdateTransform();

	template <typename T>
	T CreateChild();
	
};