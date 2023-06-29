#pragma once
#include "RenderComponent.h"

class AnimationComponent :
    public RenderComponent
{
public:
    AnimationComponent(GameObject* gameObject);
    virtual ~AnimationComponent();

public:
    virtual void Update() override;
    virtual void Render(D2DRenderer* renderer) override;

    void SetAnimationAssetPath(const std::wstring& strAssetKey) { m_strAnimationAssetPath = strAssetKey; }

public:
    std::wstring m_strAnimationAssetPath;
    AnimationInstance* m_pAnimationInstance;
    AnimationAsset* m_pAnimationAsset;

};

