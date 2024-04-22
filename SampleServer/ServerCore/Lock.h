#pragma once
#include "Types.h"

/// <summary>
/// Reader Writer SpinLock
/// 32비트 변수를 사용. 
/// 상위 16비트는 WriteFlag -> Lock을 획득하고 있는 쓰레드의 ID
/// 하위 16비트는 ReadFlag -> 공유해서 사용하고 있는 Read Count
/// </summary>

class Lock
{
	enum : uint32
	{
		// 최대로 기다려줄 Tick(시간)
		ACQUIRE_TIMEOUT_TICK = 10000,
		// SpintCount을 최대 몇번 돌지
		MAX_SPIN_COUNT = 5000,
		// Flag
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_THREAD_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag;
	// 독단적으로 사용하는것이라 경합이 붙지 않음.
	uint16 _writeCount = 0;
};

