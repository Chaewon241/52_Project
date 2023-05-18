#pragma once

#include "Scene.h"
#include "SceneManager.h"

namespace catInWonderland
{
	class Button;

	class SceneStart : public Scene
	{
	public:
		virtual void Enter();
		virtual eSceneType HandleScene(); 
		virtual void Exit();

	public:
		SceneStart();
		~SceneStart();

	private:
		Button* mStartButton;
	};
}