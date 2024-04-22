#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을 때(EMPTY_FLAG) 경합해서 소유권을 얻는다.
	const int64 beginTick = ::GetTickCount64();
	// 내 스레드 id에 해당하는 숫자를 16 비트 이동시켜서 상위 16비트에 적어두는게 최종 값이다.
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		// 5000번 테스트
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			// readCount, writeCount 다 없을 때 
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				// 경합에서 이긴 상태
				_writeCount++;
				return;
			}
		}

		// 만약에 예상한 시간보다 오래 걸렸을 때.
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		// 소유권 이전
		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
}

void Lock::ReadLock()
{
}

void Lock::ReadUnlock()
{
}
