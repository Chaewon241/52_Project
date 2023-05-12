#include "WinApp.h"
#include "GameManager.h"
#include "InputSystem.h"
#include "TimeSystem.h"

#include <string>

namespace game
{
	GameManager* GameManager::instance = nullptr;
	GameManager::GameManager()	{}
	GameManager::~GameManager()	{}

	void GameManager::Initialize()
	{
		input::InitInput();
		time::InitTime();
		//render::InitRender();
	}

	void GameManager::Run()
	{
		MSG msg;

		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				if (msg.message == WM_KEYDOWN)
				{
					
				}
				else if (msg.message == WM_KEYUP)
				{
					
				}

				else
				{ 
					DispatchMessage(&msg);
				}
			}
			else
			{
				
			}
		}
	}

	GameManager* GameManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GameManager();
		}
		return instance;
	}

	void GameManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}
}