#pragma once

#include <Windows.h>

namespace catInWonderland
{
	class TimeManager
	{
	public:
		static TimeManager* GetInstance();
		static void DestroyInstance();

		void Init();
		void Update();

		inline float GetDeltaTime();

	private:
		TimeManager();
		~TimeManager() = default;

	private:
		static TimeManager* instance;

		ULONGLONG mPreviousTime;
		ULONGLONG mCurrentTime;
		float mDeltaTime;
	};

	float TimeManager::GetDeltaTime()
	{
		return mDeltaTime;
	}
}