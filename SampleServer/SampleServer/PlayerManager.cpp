#include "pch.h"
#include "PlayerManager.h"
#include "AccountManager.h"

PlayerManager GPlayerManager;

void PlayerManager::PlayerThenAccount()
{
	// player 락을 건 후에 Account도 락을 걸어준다.
	WRITE_LOCK;
	GAccountManager.Lock();
}

void PlayerManager::Lock()
{
	WRITE_LOCK;
}
