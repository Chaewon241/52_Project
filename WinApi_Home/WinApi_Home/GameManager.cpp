#include "WinApp.h"
#include "GameManager.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "RenderSystem.h"

#include <string>

namespace game
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

	Object player = { global::GetWinApp().GetWidth() / 2 , global::GetWinApp().GetHeight() / 2,
		20, 20, RGB(0, 0, 0) };

	GameManager* GameManager::instance = nullptr;
	GameManager::GameManager()	{}
	GameManager::~GameManager()	{}

	GameManager* GameManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GameManager();
		}
		return instance;
	}

	void GameManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void GameManager::Initialize()
	{
		input::InitInput();
		time::InitTime();
		render::InitRender();
	}

	void GameManager::UpdatePlayer() 
	{
		// 교수님 코드랑 다른 부분
		if (input::IsKeyDown('A'))
			player.Move(-player.speed, 0);
		else if (input::IsKeyDown('D'))
			player.Move(player.speed, 0);
		else if(input::IsKeyDown('S'))
			player.Move(0, player.speed);
		else if (input::IsKeyDown('W'))
			player.Move(0, -player.speed);
	}

	void GameManager::Update() 
	{
		m_UpdateCount++;

		time::UpdateTime();

		UpdatePlayer();

		input::ResetInput();
	}

	void GameManager::FixedUpdate()
	{
		static ULONGLONG elapsedTime;

		elapsedTime += time::GetDeltaTime();

		while (elapsedTime >= 20)			// 0.02초
		{
			m_FixedUpdateCount++;

			elapsedTime -= 20;
		}
	}

	void GameManager::Render()
	{
		render::BeginDraw();

		DrawPlayer();

		render::EndDraw();
	}

	void GameManager::DrawPlayer()
	{
		render::CreatePlayer(player.x, player.y, player.size, player.color);
		
	}

	void GameManager::Run()
	{
		MSG msg;

		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				if (msg.message == WM_KEYDOWN)
				{
					input::KeyDown(msg.wParam);
				}
				else if (msg.message == WM_KEYUP)
				{
					input::KeyUp(msg.wParam);
				}
				else
				{ 
					DispatchMessage(&msg);
				}
			}
			else
			{
				FixedUpdate();

				Update();

				Render();
			}
		}
	}

	
}