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
			return false;

		// Exception X
		value = oldHead->data;

		// 멀티쓰레딩환경에서는 이렇게 메모리해제를 해버리면 댕글링이 일어날 수 있음.
		//delete oldHead;



		return true;
	}


private:
	atomic<Node*> _head;
};