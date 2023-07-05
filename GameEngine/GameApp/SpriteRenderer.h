#pragma once
#include "RenderComponent.h"

class D2DRenderer;
class Transform;
class Sprite;

class SpriteRenderer :
    public RenderComponent
{
public:
    SpriteRenderer(GameObject* gameObject);
public:
    void SetSprite(Sprite* sprite) { m_Sprite = sprite; }
    virtual void Render(D2DRenderer* renderer) override;
};
