#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

// �������� ����� �Ŵ���
ThreadManager* GThreadManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		// �����ϰ� �Ҹ��ϴ� ������ ���������� ���� �־ 
	// ������(main)���� �����ϴ°� �ƴ϶� �����ڿ��� �������ش�.
		GThreadManager = new ThreadManager();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
	}
}GCoreGlobal;
// ������ü�� �����