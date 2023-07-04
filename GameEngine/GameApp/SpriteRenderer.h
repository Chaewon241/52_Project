#pragma once
#include "RenderComponent.h"

class D2DRenderer;
class Transform;


class SpriteRenderer :
    public RenderComponent
{
public:
    SpriteRenderer(GameObject* gameObject);
    ~SpriteRenderer();
public:
    virtual void Render(D2DRenderer* renderer) override;
};
