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

		void ChangeScene(); //���� �ٲ� �� ���� �Լ�
		void DeleteAll();

	public:
		GameScene();
		~GameScene();
	};
}

