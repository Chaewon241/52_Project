#pragma once

namespace catInWonderland
{
	class GameCore
	{
	public:
		GameCore();

		~GameCore();

		static GameCore* GetInstance();

		static void DestroyInstance();

		void Init();

		void Frame();

		void Update();

		void FixedUpdate();

		void UpdatePlayer();

		void Render();

	private:
		static GameCore* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}