#pragma once

#include <vector>

#include "eSceneType.h"

namespace catInWonderland
{
	class Object;

	class Scene
	{
	public:
		virtual void Enter() = 0;
		virtual eSceneType HandleScene() = 0;

		virtual void Update();
		virtual void Render();
		virtual void Exit();

		inline void RegisterObject(Object* object);
		inline eSceneType GetSceneType() const;

	public:
		Scene();
		virtual ~Scene();

	protected:
		eSceneType mSceneType;

	private:
		enum { OBJECT_CAPACITY = 256 };

		std::vector <Object*> mObjects;
	};

	void Scene::RegisterObject(Object* object)
	{
		mObjects.push_back(object);
	}

	eSceneType Scene::GetSceneType() const
	{
		return mSceneType;
	}
}