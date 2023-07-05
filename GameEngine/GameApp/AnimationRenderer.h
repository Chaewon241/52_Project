#pragma once
#include "AnimationClip.h"
#include "RenderComponent.h"
#include <vector>

class AnimationRenderer :
    public RenderComponent
{
private:
    int frameIndex = -1;
    std::wstring m_anmationPath;
    std::wstring m_animationState;
    std::vector<FRAME_INFO> m_curFrame;
public:
    AnimationRenderer(GameObject* gameObject);

    void Update();
    void SetAnimation(AnimationClip* aniClip) { m_AnimationClip = aniClip; }
    void SetAnimationPath(std::wstring Path) { m_anmationPath = Path; }
    void SetAnimationState(std::wstring state) 
    { 
        m_animationState = state;
        for (int i = 0; i < m_AnimationClip->m_Animations.size(); i++)
        {
            if (m_AnimationClip->m_Animations[i].m_Name == m_animationState)
            {
                m_curFrame = m_AnimationClip->m_Animations[i].m_Frames;
            }
        }
    }
    void Render(D2DRenderer* renderer) override;
};

