#pragma once
#include <windows.h>
#include <list>

#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

namespace global
{
	namespace time
	{
		extern ULONGLONG previousTime;
		extern ULONGLONG currentTime;
		extern ULONGLONG deltaTime;

		extern int updateCount;
		extern int fixedUpdateCount;
		

		void InitTime();
		void UpdateTime();
		ULONGLONG GetDeltaTime();
	};

	extern COORD prePlayerPos; // ���� �÷��̾� ��ġ
	extern COORD curPlayerPos; // ���� �÷��̾� ��ġ
	extern COORD enemyWorldBasis;
	extern SMALL_RECT playerMovableRect;
	extern const int playerMoveSpeed;

	extern Player OnePlayer;

	extern bool RequestResetGame;

	extern std::list<Enemy> EnemyContainer;
	extern std::list<Bullet> EnemyBulletContainer;
	extern std::list<Bullet> PlayerBulletContainer;
}


void Clamp(short& n, short min, short max);