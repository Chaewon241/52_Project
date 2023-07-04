#pragma once
#include "../GameApp/GameApp.h"
#include <vector>

class GameObject;
class AnimationClip;
class Animator;
class Sprite;

// ���Ӿ��� ��ӹ޾� �ʱ�ȭ, ������Ʈ, ������ �����Ѵ�.
class DemoApp : public GameApp
{
public:
	std::vector<GameObject*> gameObjectList;
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	// virtual -> �� �� ������ �� �� �־�
	// override -> �� �� �������ҰŴ�
	bool Initialize() override;
	void Update();
	void Render();
};