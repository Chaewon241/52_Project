#include "pch.h"
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <iostream>
#include <thread>

CoreGlobal Core;

void ThreadMain()
{
	while (true)
	{
		cout << "Hello!! I am Thread..." << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadMain);
	}
}
