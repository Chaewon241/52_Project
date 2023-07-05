#pragma once
#include "RenderComponent.h"
#include <vector>
class AnimationClip;

class AnimationRenderer :
    public RenderComponent
{
private:
    AnimationClip* m_AnimationClip;
    std::vector<int> aniVec;
public:
    AnimationRenderer(GameObject* gameObject);

    void Update();
    void SetAnimation(AnimationClip* aniClip) { m_AnimationClip = aniClip; }
    void Render(D2DRenderer* renderer) override;
};

