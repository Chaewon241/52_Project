#pragma once

#include "resource.h"
#include "../D2DRenderer/GameApp.h"

class DemoApp : public GameApp
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	virtual void Update();
	virtual void Render();
	virtual bool Initialize() override;//
private:

	ID2D1Bitmap* m_pBitmap1;
	ID2D1Bitmap* m_pBitmap2;
};

