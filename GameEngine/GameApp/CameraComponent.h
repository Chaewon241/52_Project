#pragma once
#include "Transform.h"
#include "AABB.h"

class D2DRenderer;

class CameraComponent :
    public Transform
{
public:
    CameraComponent(GameObject* gameObject);

public:
    virtual void Update() override;
    void SetCamera(CameraComponent* cc) { m_pCamera = cc; }
    void Render(D2DRenderer* renderer) override;
  
    void InvertTransform(); // 역행렬을 반환.
    void GetScreenTransform();

private:
    CameraComponent* m_pCamera;
    D2D1_VECTOR_2F posVector;

    static AABB m_CameraAABBbox;
    friend class RenderComponent;
};