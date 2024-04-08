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

	// ��Ƽ������ ȯ�濡���� Empty�� ȿ�� ����.
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
	// ���ο� ��� ���� ���� �־��ְ� �������ִ� ��
	/*
	1) �� ��带 �����
	2) �� ����� next = head
	3) head = �� ���
	*/

	// �̱۾������� ���⼭ �������ص� ��.
	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;

		// 2-1. �׷��� ��ġ�⸦ �ߴ��� Ȯ���Ѵ�.
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

		// 2-2. ��ġ�� �ߴ��� Ȯ��
		// ������ ���ϴٰ� ������ �� ��, �׷��� ��Ⱑ ��û �ɸ��� ��.
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			//node->next = _head;
		}

		// 1. ��Ƽ�������� �� ���⼭ ��ġ�⸦ ���� �� �־ head�� �ٸ� �ַ� �� �� �ִ�.
		//_head = node;
	}

	/*
	1) head �б�
	2) head->next �б�
	3) head = head->next;
	4) data �����ؼ� ��ȯ
	5) ������ ��带 ����
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
			// ���и� �ϸ� �� ���ֱ� ������ ���� �� ����� ��.
			//oldHead = _head;
		}

		if (oldHead == nullptr)
			return false;

		// Exception X
		value = oldHead->data;

		// ��Ƽ������ȯ�濡���� �̷��� �޸������� �ع����� ��۸��� �Ͼ �� ����.
		//delete oldHead;



		return true;
	}


private:
	atomic<Node*> _head;
};