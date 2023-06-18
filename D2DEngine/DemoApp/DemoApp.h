#pragma once

#include "resource.h"
#include "../D2DRenderer/GameApp.h"
#include "../D2DRenderer/AnimationAsset.h"
#include "../D2DRenderer/AnimationInstance.h"

class DemoApp : public GameApp
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	virtual void Update();
	virtual void Render();
	virtual bool Initialize() override;

private:
	AnimationInstance m_Background;
	AnimationInstance m_Character;
};

