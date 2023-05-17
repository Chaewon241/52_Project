#pragma once

#include <cassert>
#include <map>

#include "eSceneType.h"

namespace catInWonderland
{
	class Scene;

	class SceneManager
	{
	public:
		static SceneManager* GetInstance();
		static void DestroyInstance();

		void Update();

		inline void RegisterScene(eSceneType sceneType, Scene* scene);

	private:
		SceneManager();
		~SceneManager();

	private:
		static SceneManager* instance;

		std::map<eSceneType, Scene*> mSceneMap;
		Scene* mCurrentScene;
	};

	void SceneManager::RegisterScene(eSceneType sceneType, Scene* scene)
	{
		assert(mSceneMap.find(sceneType) == mSceneMap.end());

		mSceneMap.emplace(sceneType, scene);
	}
}
