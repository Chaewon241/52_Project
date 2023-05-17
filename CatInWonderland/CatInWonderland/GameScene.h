#pragma once

#include "Scene.h"

namespace catInWonderland
{
	class GameScene : public Scene
	{
	public:
		virtual void Enter();
		virtual void Update();
		virtual void Exit();

		void ChangeScene(); //신을 바꿀 때 쓰는 함수
		void DeleteAll();

	public:
		GameScene();
		~GameScene();
	};
}

