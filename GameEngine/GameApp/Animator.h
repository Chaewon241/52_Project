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
    //virtual void Update() override;
    void Play(const std::string& name);
    void SetAnimationPath(const std::wstring& strAssetKey) { m_AnimationPath = strAssetKey; }
    void SetAnimationClip(AnimationClip* ac) { m_AnimationClips = ac; }
public:
    std::wstring m_AnimationPath;
    static Animator* m_AnimatorInstance;
    static AnimationClip* m_AnimationClips;
};