#pragma once
#include "Object.h"

class AnimationComponent;
class AnimationAsset;
class D2DRenderer;

class AnimationInstance :
    public Object
{

protected:
	AnimationAsset* m_pAnimationAsset;
	AnimationComponent* m_pAnimationComponent;
	size_t m_AnimationIndex;
	size_t m_FrameIndex;
	float m_ProgressTime;
	D2D1_RECT_F m_SrcRect;
	D2D1_RECT_F m_DstRect;
	bool m_bMirror;
	float m_Speed;
	D2D1_MATRIX_3X2_F m_RenderTransform;

public:
    AnimationInstance();
    virtual ~AnimationInstance();

public:
	void SetProgressTime(float val) { m_ProgressTime = val; }
	void SetSpeed(float val) { m_Speed = val; }
	void SetAnimationInfo(AnimationAsset* pAnimationInfo);
	void Update();
	void Render(D2DRenderer* renderer) override;
	void SetAnimationIndex(size_t index, bool Mirror);

	void SetAnimationComponent(AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }
};

