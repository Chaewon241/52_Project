#include <cmath>

#include "TimeManager.h"
#include "WinApp.h"

namespace catInWonderland
{
	ULONGLONG previousTime;
	ULONGLONG currentTime;
	float deltaTime;


	TimeManager* TimeManager::instance = nullptr;
	TimeManager::TimeManager() {}
	TimeManager::~TimeManager() {}	

	TimeManager* TimeManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new TimeManager();
		}
		return instance;
	}

	void TimeManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void TimeManager::Init()
	{
		previousTime = currentTime = GetTickCount64();
	}

	void TimeManager::UpdateTime()
	{
		previousTime = currentTime;

		currentTime = GetTickCount64();

		deltaTime = currentTime - previousTime;
	}

	const float TimeManager::GetDeltaTime() { return deltaTime; }
}