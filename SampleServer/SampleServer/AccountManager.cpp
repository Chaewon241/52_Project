#include "pch.h"
#include "AccountManager.h"
#include "PlayerManager.h"

AccountManager GAccountManager;

void AccountManager::AccountThenPlayer() 
{
	// Account 먼저 락을 잡고 player를 락하기
	WRITE_LOCK;
	GPlayerManager.Lock();
}

void AccountManager::Lock()
{
	WRITE_LOCK;
}