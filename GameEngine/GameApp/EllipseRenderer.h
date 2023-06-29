#pragma once
#include "RenderComponent.h"

class D2DRenderer;
class Transform;

class EllipseRenderer :
    public RenderComponent
{
private:
    float width;
    float height;

public:
    EllipseRenderer(GameObject* gameObject);
    ~EllipseRenderer();

public:
    virtual void Render(D2DRenderer* renderer) override;
};
