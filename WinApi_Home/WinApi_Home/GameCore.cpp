#include <string>

#include "GameCore.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "WinApp.h"

namespace catInWonderland
{
	struct Object
	{
		float x;
		float y;
		float size = 10;
		float speed;

		COLORREF color;

		void Move(float x, float y)
		{
			this->x += x;
			this->y += y;
		}
	};

	Object player = { catInWonderland::WinApp::GetInstance()->GetWidth() / 2 , catInWonderland::WinApp::GetInstance()->GetHeight() / 2,
		20, 20, RGB(0, 0, 0) };

	GameCore* GameCore::instance = nullptr;
	GameCore::GameCore()	{}
	GameCore::~GameCore()	{}

	GameCore* GameCore::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GameCore();
		}
		return instance;
	}

	void GameCore::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void GameCore::Init()
	{
		catInWonderland::InputManager::GetInstance()->Init();
		catInWonderland::TimeManager::GetInstance()->Init();
		catInWonderland::RenderManager::GetInstance()->Init();
		catInWonderland::SceneManager::GetInstance()->Init();
	}

	// 이거 추가
	void GameCore::Frame()
	{
		GameCore::FixedUpdate();

		GameCore::Update();

		GameCore::Render();
	}

	void GameCore::UpdatePlayer()
	{
		// 교수님 코드랑 다른 부분
		if (catInWonderland::InputManager::GetInstance()->IsKeyDown('A'))
			player.Move(-player.speed, 0);
		else if (catInWonderland::InputManager::GetInstance()->IsKeyDown('D'))
			player.Move(player.speed, 0);
		else if(catInWonderland::InputManager::GetInstance()->IsKeyDown('S'))
			player.Move(0, player.speed);
		else if (catInWonderland::InputManager::GetInstance()->IsKeyDown('W'))
			player.Move(0, -player.speed);
	}

	void GameCore::Update()
	{
		m_UpdateCount++;

		catInWonderland::TimeManager::GetInstance()->UpdateTime();

		UpdatePlayer();

		catInWonderland::InputManager::GetInstance()->ResetInput();
	}

	void GameCore::FixedUpdate()
	{
		static ULONGLONG elapsedTime;

		elapsedTime += catInWonderland::TimeManager::GetInstance()->GetDeltaTime();

		while (elapsedTime >= 20)			// 0.02초
		{
			m_FixedUpdateCount++;

			elapsedTime -= 20;
		}
	}

	void GameCore::Render()
	{
		catInWonderland::RenderManager::GetInstance()->BeginDraw();

		DrawPlayer();

		catInWonderland::RenderManager::GetInstance()->EndDraw();
	}

	void GameCore::DrawPlayer()
	{
		catInWonderland::RenderManager::GetInstance()->CreatePlayer(player.x, player.y, player.size);
	}	
}