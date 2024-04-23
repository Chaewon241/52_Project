#pragma once
#include "Types.h"

/// <summary>
/// Reader Writer SpinLock
/// 32��Ʈ ������ ���. 
/// ���� 16��Ʈ�� WriteFlag -> Lock�� ȹ���ϰ� �ִ� �������� ID
/// ���� 16��Ʈ�� ReadFlag -> �����ؼ� ����ϰ� �ִ� Read Count
/// </summary>


// W -> W (O) ���� ������ ������ Write�� �� Write ����. �ٵ� �ٸ� ������� �ȴ�.
// W -> R (O) Write�� ���¿��� Read ����
// R -> W (X)
// 

class Lock
{
	enum : uint32
	{
		// �ִ�� ��ٷ��� Tick(�ð�)
		ACQUIRE_TIMEOUT_TICK = 10000,
		// SpintCount�� �ִ� ��� ����
		MAX_SPIN_COUNT = 5000,
		// Flag
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag;
	// ���������� ����ϴ°��̶� ������ ���� ����.
	uint16 _writeCount = 0;
};

/*---------------
	LockGuards
----------------*/

class ReadLockGuard
{
public:
	// ��ü�� ��������� Lock����
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	// ��ü�� �Ҹ�ɶ� UnLock����
	~ReadLockGuard() { _lock.ReadUnlock(); }

private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	// ��ü�� ��������� Lock����
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	// ��ü�� �Ҹ�ɶ� UnLock����
	~WriteLockGuard() { _lock.WriteUnlock(); }

private:
	Lock& _lock;
};