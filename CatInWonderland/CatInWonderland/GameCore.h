#pragma once

namespace catInWonderland
{
	class GameCore
	{
	public:
		static GameCore* GetInstance();
		static void DestroyInstance();

		void Frame();

	private:
		GameCore();
		~GameCore();

	private:
		static GameCore* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}