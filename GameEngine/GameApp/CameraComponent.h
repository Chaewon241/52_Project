#pragma once
#include "RenderComponent.h"
#include "Transform.h"

class CameraComponent :
    public RenderComponent
{
    CameraComponent(GameObject* gameObject);

public:
    virtual void Update() override;
};

