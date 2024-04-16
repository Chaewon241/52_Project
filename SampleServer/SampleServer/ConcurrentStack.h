#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);

		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();

		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] { return _stack.empty() == false; });
		value = std::move(_stack.pop());
	}

	// 멀티쓰레드 환경에서는 Empty가 효과 없음.
	/*bool Empty()
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
	}*/

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value)
		{

		}
		T data;
		Node* next;
	};

public:
	// 새로운 노드 만들어서 값을 넣어주고 연결해주는 것
	/*
	1) 새 노드를 만들고
	2) 새 노드의 next = head
	3) head = 새 노드
	*/

	// 싱글쓰레드라면 여기서 마무리해도 됨.
	// Push는 나만 쓰는거, 그리고 노드를 새로 만들어서 넣어주는 것이기 때문에
	// 문제 될 것은 없음.
	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;

		// 2-1. 그래서 새치기를 했는지 확인한다.
		/*if (_head == node->next)
		{
			_head = node;
			return true;
		}
		else
		{
			node->next = _head;
			return false;
		}*/

		// 2-2. 새치기 했는지 확인
		// 경합이 심하다고 가정을 할 때, 그러면 대기가 엄청 걸리게 됨.
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			//node->next = _head;
		}

		// 1. 멀티쓰레드일 때 여기서 새치기를 당할 수 있어서 head가 다른 애로 올 수 있다.
		//_head = node;
	}

	/*
	1) head 읽기
	2) head->next 읽기
	3) head = head->next;
	4) data 추출해서 반환
	5) 추출한 노드를 삭제
	*/

	// TryPop은 Head 막 참조하고 그래서 나 혼자 쓰는게 아님.
	// 그래서 문제가 될 수도 있음.
	bool TryPop(T& value)
	{
		Node* oldHead = _head;

		/*if (_head == oldHead)
		{
			_head = oldHead->next;
			return true;
		}
		else
		{
			oldHead = _head;
			return false;
		}*/

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
			// 실패를 하면 안 해주기 때문에 굳이 안 써놔도 됨.
			//oldHead = _head;
		}

		if (oldHead == nullptr)
		{
			// #2 참조 카운트 줄여주기
			--_popCount;
			return false;
		}

		// Exception X
		value = oldHead->data;

		// 멀티쓰레딩환경에서는 이렇게 메모리해제를 해버리면 댕글링이 일어날 수 있음.
		//delete oldHead;

		// #2 
		TryDelete(oldHead);

		return true;
	}

	// #2 
	/*
	1) 데이터 분리
	2) Count 체크
	3) 나 혼자면 삭제
	*/
	// #2
	void TryDelete(Node* oldHead)
	{
		// 나 외에 누가 pop 하려고 하는가
		if (_popCount == 1)
		{
			// 나 혼자 pop 하네?

			// 이왕 혼자인거 삭제 예약된 다른 데이터들도 삭제하기.
			Node* node = _pendingList.exchange(nullptr);

			// 아토믹으로 선언해놔서 --를 해도 아토믹하게 실행됨.
			if (--_popCount == 0)
			{
				// 끼어든 애가 없음 -> 삭제 진행
				// 이제와서 끼어들어도 어차피 데이터는 분리해둔 상태~!
				DeleteNodes(node);
			}
			else if(node)
			{
				// 누가 끼어들었으니 다시 갖다 놓자.
				ChainPendingNodeList(node);
			}

			// 내 데이터는 삭제
			delete oldHead;
		}
		else
		{
			// 누가 있네? 그럼 지금 삭제하지 않고, 삭제 예약만
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	// #2
	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;

		// 누군가가 끼어든다면?!
		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{

		}
	}

	// #2
	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		// last 찾기
		while (last->next)
		{
			last = last->next;
		}
		ChainPendingNodeList(node, last);
	}

	// #2
	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node);
	}

	// #2
	static void DeleteNodes(Node* node)
	{
		// 연결된 노드들 다 지우기
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}


private:
	atomic<Node*> _head;


	// #2 삭제되어야할 노드들(첫번째 노드)
	atomic<Node*> _pendingList;
	// #2 pop을 실행중인 쓰레드 개수
	atomic<int32> _popCount = 0;
};