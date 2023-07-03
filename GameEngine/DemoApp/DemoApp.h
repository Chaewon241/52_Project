#pragma once
#include "../GameApp/GameApp.h"
#include <vector>

class GameObject;
class AnimationClip;

// ���Ӿ��� ��ӹ޾� �ʱ�ȭ, ������Ʈ, ������ �����Ѵ�.
class DemoApp : public GameApp
{
public:
	std::vector<GameObject*> gameObjectList;
	AnimationClip* m_pAnimationAsset;
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	// virtual -> �� �� ������ �� �� �־�
	// override -> �� �� �������ҰŴ�
	bool Initialize() override;
	void Update();
	void Render();
};

