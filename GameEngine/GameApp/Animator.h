#pragma once
#include "Component.h"

class D2DRenderer;
class AnimationClip;

class Animator :
    public Component
{
public:
    Animator(GameObject* gameObject);
    ~Animator();

public:
    virtual void Update() override;
    void Play(const std::string& name);
    void SetAnimationAsset(const std::wstring& strAssetKey);

public:
    std::wstring m_AnimationKey;
    AnimationClip* m_AnimationClips;
};