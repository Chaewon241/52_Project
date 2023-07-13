#pragma once
#include "RenderComponent.h"

class D2DRenderer;

class CameraComponent :
    public RenderComponent
{
public:
    CameraComponent(GameObject* gameObject);

public:
    virtual void Update() override;
    void SetCamera(CameraComponent* cc) { m_pCamera = cc; }
    void Render(D2DRenderer* renderer) override;
private:
    CameraComponent* m_pCamera;
};