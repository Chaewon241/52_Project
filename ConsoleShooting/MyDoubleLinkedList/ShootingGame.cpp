// MyDoubleLinkedList.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <list>

#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"


#include <stdio.h>
#include <windows.h>
#include <random>

#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!
#include "RenderSystem.h"
#include "Global.h"





void StartGame()
{
	render::InitScreen();

	global::playerMovableRect = render::GetPlayerMovableRect();

	render::DrawBorder(); // 벽을 그려 놓자!

	global::OnePlayer.Reset();


}

void SpawnEnemy()
{
	Enemy Info;

	COORD pos;
	pos.X = render::GetPlayerMovableRect().Left + rand() % (render::GetPlayerMovableRect().Right - render::GetPlayerMovableRect().Left) - 2;
	pos.Y = render::GetPlayerMovableRect().Top + rand() % (render::GetPlayerMovableRect().Bottom - render::GetPlayerMovableRect().Top) - 2;

	Info.SetPosition(pos);
	float dirX = 1.0f, dirY = 1.0f;
	if (rand() % 2)
	{
		dirX = dirX * -1.0f;
	}
	if (rand() % 2)
	{
		dirY = dirY * -1.0f;
	}
	Info.SetDirection(dirX, dirY);
	global::EnemyContainer.push_front(Info);

}

void EndGame()
{
	render::ScreenRelease();
}

void ProcessInput()
{
	global::input::UpdateInput();
}

void PrintCountsPerSecond();
void PrintListInfo();

void Render()
{
	render::ScreenClear();

	PrintCountsPerSecond();

	PrintListInfo();

	global::OnePlayer.Draw();

	std::list<Bullet>::iterator iter_bullet = global::PlayerBulletContainer.begin();
	while (iter_bullet != global::PlayerBulletContainer.end())
	{
		iter_bullet->Draw();
		iter_bullet++;
	}

	std::list<Bullet>::iterator iter_eb = global::EnemyBulletContainer.begin();
	while (iter_eb != global::EnemyBulletContainer.end())
	{
		iter_eb->Draw();
		iter_eb++;
	}
	
	std::list<Enemy>::iterator iter_enemy = global::EnemyContainer.begin();
	while (iter_enemy != global::EnemyContainer.end())
	{
		iter_enemy->Draw();
		iter_enemy++;
	}

    render::DrawBorder();
    render::ScreenFlipping();
}

bool IsGameRun()
{
    return true;
}

void UpdateSpawn(ULONGLONG fixedTime)
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= fixedTime) //1초
    {
        global::time::fixedUpdateCount += 1;

        elapsedTime -= fixedTime;

	    

		SpawnEnemy();

    }
}

void ResetGame()
{
	global::OnePlayer.Reset();

	global::EnemyContainer.clear();
	global::EnemyBulletContainer.clear();
	global::PlayerBulletContainer.clear();
}

void Update()
{
	if (global::RequestResetGame)
	{
		ResetGame();
		global::RequestResetGame = false;
	}
	UpdateSpawn(5000);

    global::time::updateCount += 1;
    global::OnePlayer.Update();

	std::list<Bullet>::iterator iter_bullet = global::PlayerBulletContainer.begin();
	while (iter_bullet != global::PlayerBulletContainer.end())
	{
		iter_bullet->Update();
		
		if (iter_bullet->waitingDestroy)
		{
			iter_bullet = global::PlayerBulletContainer.erase(iter_bullet);
		}
		else
			iter_bullet++;
	}

	std::list<Bullet>::iterator iter_eb = global::EnemyBulletContainer.begin();
	while (iter_eb != global::EnemyBulletContainer.end())
	{
		iter_eb->Update();

		if (iter_eb->waitingDestroy)
		{
			iter_eb = global::EnemyBulletContainer.erase(iter_eb);
		}
		else
			iter_eb++;
	}

	std::list<Enemy>::iterator iter_enemy = global::EnemyContainer.begin();
	while (iter_enemy != global::EnemyContainer.end())
	{
		iter_enemy->Update();

		if (iter_enemy->waitingDestroy)
		{
			iter_enemy = global::EnemyContainer.erase(iter_enemy);
		}
		else
			iter_enemy++;
	}

}

void PrintListInfo()
{
	char buffer[70] = "";
	sprintf_s(buffer, "Enemy %2d, EnemyBullet %2d , PlayerBullet %2d , Score %5d", 
		global::EnemyContainer.size(),
		global::EnemyBulletContainer.size(),
		global::PlayerBulletContainer.size(),
		global::OnePlayer.Score
		);

	render::ScreenDraw(50, 0, buffer);
}

void PrintCountsPerSecond()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();
    if (elapsedTime >= 1000)
    {
        char buffer[64] = "";

        sprintf_s(buffer, "updateCount : %2d fixedUpdateCount : %2d  delta %2d\n",
            global::time::updateCount, global::time::fixedUpdateCount, global::time::GetDeltaTime());

        render::ScreenDraw(0, 0, buffer);

        elapsedTime = 0;
        global::time::updateCount = 0;
        global::time::fixedUpdateCount = 0;
    }
}

int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();		
        Update();
        Render();
    }

    EndGame();

    return 0;
}

