#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"

// 전역으로 사용할 매니저
ThreadManager* GThreadManager = nullptr;
Memory* GMemory = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		// 생성하고 소멸하는 순서가 정해져야할 수도 있어서 
		// 컨텐츠(main)에서 생성하는게 아니라 생성자에서 생성해준다.
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GDeadLockProfiler = new DeadLockProfiler();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GDeadLockProfiler;
	}
}GCoreGlobal;
// 전역객체로 만들기