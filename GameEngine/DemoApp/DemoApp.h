#pragma once
#include "../GameApp/GameApp.h"
#include <vector>

class GameObject;
class AnimationClip;
class Animator;
class Sprite;

// 게임앱을 상속받아 초기화, 업데이트, 렌더를 실행한다.
class DemoApp : public GameApp
{
public:
	std::vector<GameObject*> gameObjectList;
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	// virtual -> 너 나 재정의 할 수 있어
	// override -> 나 너 재정의할거다
	bool Initialize() override;
	void Update();
	void Render();
};