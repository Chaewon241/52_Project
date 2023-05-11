#pragma once

namespace game
{
	class GameManager
	{
	public:
		GameManager();

		~GameManager();

		void Run();

		void Initialize();

		static GameManager* GetInstance();

		static void DestroyInstance();

	private:
		static GameManager* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}