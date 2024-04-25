#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// ���̵� ã�ų� �߱��Ѵ�.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);

	// ���࿡ ã�� ID�� ����.
	if (findIt == _nameToId.end())
	{
		// �׷��� �߱�
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// ��� �ִ� ���� �־��ٸ�
	if (!_lockStack.empty())
	{
		// ������ �߰ߵ��� ���� ���̽���� ����� ���θ� �ٽ� Ȯ���Ѵ�.
		const int32 prevId = _lockStack.top();
		if (lockId != prevId)
		{
			// �ٸ� ���� ��Ҵٸ� �����丮 ����
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				// ���ο� ������ �߰��� ���̶�� ������ �߰����ش�.
				history.insert(lockId);
				CheckCycle();
			}
		}
	}
	// ��� �ִ� ���� ������ �����ÿ� �־��ֱ�
	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (_lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	// name�� ������ �������� ��..
	int32 lockId = _nameToId[name];
	if (_lockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	// ���ݱ��� �߰ߵ� lock ī��Ʈ
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	// �α׸� �ʱ�ȭ, -1�� ���ִ� ���� = ������ �ƿ� �鸮�� �ʾ��� ��
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	// �ش� dfs�� �Ϻ��ϰ� ��������
	_finished = vector<bool>(lockCount, false);
	// � ��ο� ���� �߰��� �ƴ���
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// ������ �������� �����Ѵ�.
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if (_discoveredOrder[here] != -1)
		return;

	_discoveredOrder[here] = _discoveredCount++;

	// ��� ������ ������ ��ȸ�Ѵ�.
	// lockHistory�� ���� �������� ������ Ÿ��Ÿ�� �� �� �ִ�.
	auto findIt = _lockHistory.find(here);
	if (findIt == _lockHistory.end())
	{
		// ���� ���� ���¿��� �ٸ� ���� ���� ���� ���ٸ�
		_finished[here] = true;
		return;
	}

	// ���� ���� ���¿��� �ٸ� ���� ���� ���� �ִٸ�
	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// ���� �湮�� ���� ���ٸ� �湮�Ѵ�.
		if (_discoveredOrder[there] == -1)
		{
			// �ʴ� �������� �湮�� �Ȱ�
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// ���������� ���������� Ȯ���ϱ�
		// here�� there���� ���� �߰ߵǾ��ٸ�, there�� here�� �ļ��̴�. (������ ����)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// �������� �ƴϰ�, Dfs(there)�� ���� �������� �ʾҴٸ�, there�� here�� �����̴�. (������ ����)
		if (_finished[there] == false)
		{
			// ����� ������ ũ������ ���� �����̶� �ѹ� ����Ǿ��ִ� ��� Ȯ���ϱ�.
			printf("%s -> %s\n", _idToName[here], _idToName[there]);
			
			// �θ� ��� Ÿ�� �ö�.
			int32 now = here;

			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}