#pragma once
#include "../GameApp/GameApp.h"

// ���Ӿ��� ��ӹ޾� �ʱ�ȭ, ������Ʈ, ������ �����Ѵ�.
class DemoApp : public GameApp
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	// virtual -> �� �� ������ �� �� �־�
	// override -> �� �� �������ҰŴ�
	bool Initialize() override;
	void Render();
};

