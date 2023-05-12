#include "WinApp.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "RenderSystem.h"

#include "GameManager.h"

#include <string>

namespace game
{
	struct Object
	{
		float x;
		float y;
		float size;
		float speed;

		COLORREF color;

		void SetPos(float x, float y)
		{

			this->x = x;
			this->y = y;
		}

		void Move(float x, float y)
		{
			this->x += x;
			this->y += y;
		}

		void MoveByDeltaTime(float x, float y)
		{
			this->x += x * time::GetDeltaTime();
			this->y += y * time::GetDeltaTime();
		}
	};

	// 중앙에 있게 하기 위해서 위치 조정
	// 이거 속도 바꿔줘야함!!
	Object player = { global::GetWinApp().GetWidth() / 2 ,global::GetWinApp().GetHeight() / 2, 10, 1, RGB(255, 255, 0) };
#
	const int bludeCircleMax = 5;
	int blueCircleCount = 0;
	Object blueCircles[bludeCircleMax];
	HBITMAP hTestBitmap = nullptr; // 전역 변수

	void UpdatePlayer()
	{
		// 게임 로직은 여기에 추가
		if (input::IsKey('A'))
		{
			player.MoveByDeltaTime(-player.speed, 0);
		}
		else if (input::IsKey('D'))
		{
			player.MoveByDeltaTime(player.speed, 0);
		}
		if (input::IsKey('W'))
		{
			player.MoveByDeltaTime(0, -player.speed);
		}
		else if (input::IsKey('S'))
		{
			player.MoveByDeltaTime(0, player.speed);
		}
	}

	void UpdateBlueCircle()
	{
		const input::MouseState& mouse = input::GetMouseState();
		const input::MouseState& prevmouse = input::GetPrevMouseState();

		// 현재 마우스 상태와 이전 마우스 상태가 같을 때
		if (input::IsSame(mouse, prevmouse))
		{
			return;
		}

		if (blueCircleCount < bludeCircleMax && mouse.left)
		{
			blueCircles[blueCircleCount].SetPos(mouse.x, mouse.y);
			blueCircles[blueCircleCount].color = RGB(0, 0, 255);
			blueCircles[blueCircleCount].size = 10;
			blueCircles[blueCircleCount].speed = 0;
			blueCircleCount++;
		}
	}

	// static 변수는 멤버와 달리 외부에 있어야한다.
	GameManager* GameManager::instance = nullptr;
	GameManager::GameManager()
	{
	}
	GameManager::~GameManager()
	{
	}

	void GameManager::Initialize()
	{
		input::InitInput();
		time::InitTime();
		render::InitRender();
	}

	void GameManager::Update()
	{
		++m_UpdateCount;

		input::UpdateMouse();

		// 업데이트에서 하는거 아님 아래 run에서 함
		//time::UpdateTime();

		UpdatePlayer();

		UpdateBlueCircle();

		input::ResetInput();

	}

	void GameManager::FixeUpdate()
	{
		static ULONGLONG elapsedTime;

		elapsedTime += time::GetDeltaTime();

		while (elapsedTime >= 20) //0.02초
		{
			++m_FixedUpdateCount;

			elapsedTime -= 20;
		}
	}

	void GameManager::Render()
	{
		render::BeginDraw();

		DrawFPS();
		DrawSomething();
		DrawPlayer();

		render::EndDraw();
	}

	void GameManager::Finalize()
	{
		render::ReleaseRender();
	}

	// load unload 추가
	void GameManager::Run()
	{
		MSG msg;

		// 게임에 필요한 리소스를 미리 로드하면 좋아용
		Load();

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
				time::UpdateTime();

				FixeUpdate();

				Update();

				Render();

			}
		}

		Unload();
	}

	void GameManager::Load()
	{
		hTestBitmap = render::LoadImdage("sample.bmp");
	}

	void GameManager::Unload()
	{
		render::ReleaseImage(hTestBitmap);
	}

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

	void GameManager::DrawFPS()
	{
		// 
		static double elapsedTime;
		static int UpdateCount;
		static int FixedUpdateCount;

		elapsedTime += time::GetDeltaTime();

		if (elapsedTime >= 1000)
		{
			// 이건 찜찜해서 0.0으로 바꿈
			elapsedTime = 0.0;
			UpdateCount = m_UpdateCount;
			FixedUpdateCount = m_FixedUpdateCount;

			m_UpdateCount = 0;
			m_FixedUpdateCount = 0;
		}

		std::string str = "FPS: " + std::to_string(time::GetFrameRate());
		str += "           Update " + std::to_string(UpdateCount);
		str += "           FixedUpdate " + std::to_string(FixedUpdateCount);

		render::DrawText(10, 10, str.c_str(), RGB(255, 0, 0));

	}

	void GameManager::DrawPlayer()
	{
		render::DrawCircle(player.x, player.y, player.size, player.color);
	}

	void GameManager::DrawSomething()
	{

		for (int i = 0; i < blueCircleCount; i++)
		{
			render::DrawCircle(blueCircles[i].x, blueCircles[i].y, blueCircles[i].size, blueCircles[i].color);
		}
		// 비트맵 추가(draw 해줌)
		render::DrawBitmap(50, 50, hTestBitmap);

		render::DrawLine(player.x - 50, player.y + 50, player.x + 50, player.y + 50, RGB(255, 0, 0));
		render::DrawRect(player.x - 25, player.y - 25, 50, 50, RGB(255, 0, 255));
	}
}