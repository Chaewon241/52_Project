#pragma once

#include "GameScene.h"
#include "BoardObject.h"

namespace catInWonderland
{
	class Player;

	class Stage01 :public GameScene
	{
	public:
		virtual void Enter();
		virtual eSceneType HandleScene();

	public:
		Stage01();
		virtual ~Stage01() = default;
	};
}