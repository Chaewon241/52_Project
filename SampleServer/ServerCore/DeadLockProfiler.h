#pragma once
#include <stack>
#include <map>
#include <vector>

/// <summary>
/// ���̺꿡�� ����� ������ �ʰ� �츮�� ���� �Ǵ��� �� �ְ�
/// �̸� Ž���� �� �ְ��ϴ� Ŭ����
/// </summary>

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 index);

private:
	// �̸��̶� �ش� ���� ��ȣ�� �����ϴ� ��
	unordered_map<const char*, int32>	_nameToId;
	// ���̵�� �̸��� �����ϴ� ��, ������ ���� �Դٰ����� �� �ְ�
	unordered_map<int32, const char*>	_idToName;
	// Lock�� ����Ǵ°� �������� �������ֱ�.
	stack<int32>						_lockStack;
	// Lock �����丮, ���� ����
	map<int32, set<int32>>				_lockHistory;

	// ��Ƽ������ ȯ�濡�� �����ϰ� ���ư� �� �ְ�
	Mutex _lock;

private:
	// ��尡 �߰ߵ� ������ ����ϴ� �迭
	vector<int32> _discoveredOrder;
	// ��尡 �߰ߵ� ����
	int32 _discoveredCount = 0;
	// Dfs�� ����Ǿ�����.
	vector<bool> _finished;
	vector<int32> _parent;

};

