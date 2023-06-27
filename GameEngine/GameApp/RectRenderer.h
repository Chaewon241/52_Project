#pragma once
#include "RenderComponent.h"

class D2DRenderer;
class Transform;

class RectRenderer :
    public RenderComponent
{
private:
    float width;
    float height;

public:
    RectRenderer(GameObject* gameObject);
    ~RectRenderer();

public:
    virtual void Render(D2DRenderer* renderer) override;
};

