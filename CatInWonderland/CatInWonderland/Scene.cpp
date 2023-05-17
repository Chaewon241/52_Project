#include "Scene.h"
#include "Object.h"

namespace catInWonderland
{
	Scene::Scene()
	{
		mObjects.reserve(OBJECT_CAPACITY);
	}

	Scene::~Scene()
	{
		Exit();
		mObjects.clear();
	}

	void Scene::Update()
	{
		for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			(*iter)->Update();
		}
	}

	void Scene::Render()
	{
		for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			(*iter)->Render();
		}
	}

	void Scene::Exit()
	{
		for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			delete (*iter);
		}
	}
}