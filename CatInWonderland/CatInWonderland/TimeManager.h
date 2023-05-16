#pragma once

namespace catInWonderland
{
	class TimeManager 
	{
	public:
		TimeManager();
		
		~TimeManager();

		static TimeManager* GetInstance();

		static void DestroyInstance();

		void Init();

		void UpdateTime();

		const float GetDeltaTime();		

	private:
		static TimeManager* instance;
	};
}