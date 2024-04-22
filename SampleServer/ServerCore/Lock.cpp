#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// �ƹ��� ���� �� �����ϰ� ���� ���� ��(EMPTY_FLAG) �����ؼ� �������� ��´�.
	const int64 beginTick = ::GetTickCount64();
	// �� ������ id�� �ش��ϴ� ���ڸ� 16 ��Ʈ �̵����Ѽ� ���� 16��Ʈ�� ����δ°� ���� ���̴�.
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		// 5000�� �׽�Ʈ
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			// readCount, writeCount �� ���� �� 
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				// ���տ��� �̱� ����
				_writeCount++;
				return;
			}
		}

		// ���࿡ ������ �ð����� ���� �ɷ��� ��.
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		// ������ ����
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
