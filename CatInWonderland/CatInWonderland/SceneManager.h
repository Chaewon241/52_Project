#pragma once

#include <cassert>
#include <map>

#include "eSceneType.h"
#include "Scene.h"

namespace catInWonderland
{
	class Scene;

	class SceneManager
	{
		friend class SceneLoader;

	public:
		static SceneManager* GetInstance();
		static void DestroyInstance();

		void Update();

		inline void RegisterScene(eSceneType sceneType, Scene* scene);
		inline const Scene& GetCurrentScene() const;

	private:
		SceneManager();
		~SceneManager();

		inline void setCurScene(eSceneType sceneType);

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

	void SceneManager::setCurScene(eSceneType sceneType)
	{
		auto iter = mSceneMap.find(sceneType);
		assert(iter != mSceneMap.end());
		mCurrentScene = iter->second;
		mCurrentScene->Enter();
	}

	const Scene& SceneManager::GetCurrentScene() const
	{
		return *mCurrentScene;
	}
}