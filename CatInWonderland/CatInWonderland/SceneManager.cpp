#include <cassert>

#include "SceneManager.h"
#include "Scene.h"
#include "WinApp.h"
#include "Stage01.h"

namespace catInWonderland
{
	SceneManager* SceneManager::instance = nullptr;

	SceneManager* SceneManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new SceneManager();
		}

		return instance;
	}

	void SceneManager::DestroyInstance()
	{
		delete instance;
		instance = nullptr;
	}
	SceneManager::SceneManager()
		: mCurrentScene(new Stage01())
	{
		mCurrentScene->Enter();
	}

	SceneManager::~SceneManager()
	{
		for (auto iter = mSceneMap.begin(); iter != mSceneMap.end(); ++iter)
		{
			delete (iter->second);
		}

		mSceneMap.clear();
	}

	void SceneManager::Update()
	{
		mCurrentScene->Update();
		mCurrentScene->Render();

		eSceneType nextSceneType = mCurrentScene->HandleScene();

		if (nextSceneType != mCurrentScene->GetSceneType())
		{
			mCurrentScene->Exit();
			
			auto iter = mSceneMap.find(nextSceneType);
			assert(iter != mSceneMap.end());
			 
			mCurrentScene = iter->second;
			mCurrentScene->Enter();
		}
	}
}