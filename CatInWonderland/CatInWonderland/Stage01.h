#pragma once

#include "GameScene.h"
#include "BoardObject.h"

namespace catInWonderland
{
	class Player;

	class Stage01 :public Scene
	{
	public:
		virtual void Enter();
		virtual eSceneType HandleScene();
		virtual void Update();
		virtual void Exit();

		inline void RegisterBoardObject(BoardObject* boardObject);

	public:
		Stage01() = default;
		~Stage01() = default;

	private:
		Player* mPlayer;
		float mElapsedTime;
		bool mbLeft;
		bool mbRotate;
		std::vector <BoardObject*> mBoardObjects;
	};

	void Stage01::RegisterBoardObject(BoardObject* boardObject)
	{
		mBoardObjects.push_back(boardObject);
		RegisterObject(static_cast<Object*>(boardObject));
	}
}