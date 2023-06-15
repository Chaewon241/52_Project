#pragma once

#include "Scene.h"
#include "BoardObject.h"

namespace catInWonderland
{
	class Player;

	class GameScene : public Scene
	{
	public:
		virtual void Enter();
		virtual eSceneType HandleScene() = 0;
		virtual void Update();
 		virtual void Exit();

		inline void RegisterBoardObject(BoardObject* boardObject);

	public:
		GameScene(unsigned int boardSize);
		~GameScene() = default;

	protected:
		Player* mPlayer;
		float mElapsedTime;
		bool mbLeft;
		bool mbRotate;
		unsigned int mBoardSize;
		std::vector <BoardObject*> mBoardObjects;
	};

	void GameScene::RegisterBoardObject(BoardObject* boardObject)
	{
		mBoardObjects.push_back(boardObject);
		RegisterObject(static_cast<Object*>(boardObject));
	}
}