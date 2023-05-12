#pragma once

namespace game
{
	class GameManager
	{
	public:
		GameManager();

		~GameManager();

		void Initialize();

		void Update();

		void FixeUpdate();

		void Render();

		void Finalize();

		void Run();

		// 요기
		void Load();
		// 요기 추가
		void Unload();

		static GameManager* GetInstance();

		static void DestroyInstance();

	private:

		void DrawFPS();
		void DrawPlayer();
		void DrawSomething();

		static GameManager* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}