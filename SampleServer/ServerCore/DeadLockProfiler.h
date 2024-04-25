#pragma once
#include <stack>
#include <map>
#include <vector>

/// <summary>
/// 라이브에서 데드락 터지지 않고 우리가 직접 판단할 수 있게
/// 미리 탐지할 수 있게하는 클래스
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
	// 이름이랑 해당 락의 번호를 관리하는 맵
	unordered_map<const char*, int32>	_nameToId;
	// 아이디랑 이름을 매핑하는 맵, 위에랑 서로 왔다갔다할 수 있게
	unordered_map<int32, const char*>	_idToName;
	// Lock이 실행되는걸 스택으로 추적해주기.
	stack<int32>						_lockStack;
	// Lock 히스토리, 간선 관리
	map<int32, set<int32>>				_lockHistory;

	// 멀티쓰레드 환경에서 안전하게 돌아갈 수 있게
	Mutex _lock;

private:
	// 노드가 발견된 순서를 기록하는 배열
	vector<int32> _discoveredOrder;
	// 노드가 발견된 순서
	int32 _discoveredCount = 0;
	// Dfs가 종료되었는지.
	vector<bool> _finished;
	vector<int32> _parent;

};

