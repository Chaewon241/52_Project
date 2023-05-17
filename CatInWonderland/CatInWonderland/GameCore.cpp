#include <string>

#include "GameCore.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "SpriteManager.h"
#include "EnumLoader.h"
#include "WinApp.h"

namespace catInWonderland
{
	GameCore* GameCore::instance = nullptr;

	GameCore* GameCore::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GameCore();
		}
		return instance;
	}

	void GameCore::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	GameCore::GameCore()
	{
		EnumLoader::LoadSprtie();

		InputManager::GetInstance();
		TimeManager::GetInstance();
		RenderManager::GetInstance();
		SceneManager::GetInstance();
		SpriteManager::GetInstance();

	}

	GameCore::~GameCore()
	{
		InputManager::DestroyInstance();
		TimeManager::DestroyInstance();
		RenderManager::DestroyInstance();
		SceneManager::DestroyInstance();
		SpriteManager::DestroyInstance();
	}

	void GameCore::Frame()
	{
		InputManager::GetInstance()->Update();
		TimeManager::GetInstance()->Update();
		SceneManager::GetInstance()->Update();

		RenderManager::GetInstance()->Render();
	}
}