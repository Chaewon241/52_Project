#pragma once

namespace game
{
	class GameManager
	{
	public:
		GameManager();

		~GameManager();

		static GameManager* GetInstance();

		static void DestroyInstance();

		void Initialize();

		void Run();

		void Update();

		void FixedUpdate();

		void UpdatePlayer();

		void Render();

		void DrawPlayer();

	private:
		static GameManager* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}