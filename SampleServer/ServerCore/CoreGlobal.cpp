#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

// �������� ����� �Ŵ���
ThreadManager* GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		// �����ϰ� �Ҹ��ϴ� ������ ���������� ���� �־ 
		// ������(main)���� �����ϴ°� �ƴ϶� �����ڿ��� �������ش�.
		GThreadManager = new ThreadManager();
		GDeadLockProfiler = new DeadLockProfiler();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
	}
}GCoreGlobal;
// ������ü�� �����