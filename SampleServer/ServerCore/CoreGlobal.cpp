#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"

// �������� ����� �Ŵ���
ThreadManager*		GThreadManager = nullptr;
Memory*				GMemory = nullptr;
DeadLockProfiler*	GDeadLockProfiler = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		// �����ϰ� �Ҹ��ϴ� ������ ���������� ���� �־ 
		// ������(main)���� �����ϴ°� �ƴ϶� �����ڿ��� �������ش�.
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GDeadLockProfiler = new DeadLockProfiler();
		// ���� �ʱ�ȭ
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GDeadLockProfiler;
		SocketUtils::Clear();
	}
}GCoreGlobal;
// ������ü�� �����