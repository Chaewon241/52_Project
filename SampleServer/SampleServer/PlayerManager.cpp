#include "pch.h"
#include "PlayerManager.h"
#include "AccountManager.h"

PlayerManager GPlayerManager;

void PlayerManager::PlayerThenAccount()
{
	// player ���� �� �Ŀ� Account�� ���� �ɾ��ش�.
	WRITE_LOCK;
	GAccountManager.Lock();
}

void PlayerManager::Lock()
{
	WRITE_LOCK;
}
