#pragma once
#include "GameObject.h"

class CameraComponent;

class Camera : public GameObject
{
public:
	Camera();
	~Camera();
	
public:
	CameraComponent* GetCameraComponent() { return m_pCameraComponent; }

private:
	CameraComponent* m_pCameraComponent;
};

