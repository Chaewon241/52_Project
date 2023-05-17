#include "TimeManager.h"
#include "WinApp.h"

namespace catInWonderland
{
	TimeManager* TimeManager::instance = nullptr;

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

	TimeManager::TimeManager()
		: mCurrentTime(GetTickCount64())
		, mPreviousTime(mCurrentTime)
		, mDeltaTime(0.f)
	{
	}

	void TimeManager::Init()
	{
		mPreviousTime = mCurrentTime = GetTickCount64();
		mDeltaTime = 0.f;
	}

	void TimeManager::Update()
	{
		mPreviousTime = mCurrentTime;
		mCurrentTime = GetTickCount64();
		mDeltaTime = (mCurrentTime - mPreviousTime) * 0.001f;
	}
}