#pragma once
#include "RenderComponent.h"
#include "AABB.h"

class D2DRenderer;
class Transform;

class RectRenderer :
    public RenderComponent
{
private:
    D2D1_RECT_F m_Rect;

public:
    RectRenderer(GameObject* gameObject);
    ~RectRenderer();

public:
    void SetExtend(float x, float y);
    virtual void Render(D2DRenderer* renderer) override;

};

