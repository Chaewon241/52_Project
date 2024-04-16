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
	// Push�� ���� ���°�, �׸��� ��带 ���� ���� �־��ִ� ���̱� ������
	// ���� �� ���� ����.
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

	// TryPop�� Head �� �����ϰ� �׷��� �� ȥ�� ���°� �ƴ�.
	// �׷��� ������ �� ���� ����.
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
		{
			// #2 ���� ī��Ʈ �ٿ��ֱ�
			--_popCount;
			return false;
		}

		// Exception X
		value = oldHead->data;

		// ��Ƽ������ȯ�濡���� �̷��� �޸������� �ع����� ��۸��� �Ͼ �� ����.
		//delete oldHead;

		// #2 
		TryDelete(oldHead);

		return true;
	}

	// #2 
	/*
	1) ������ �и�
	2) Count üũ
	3) �� ȥ�ڸ� ����
	*/
	// #2
	void TryDelete(Node* oldHead)
	{
		// �� �ܿ� ���� pop �Ϸ��� �ϴ°�
		if (_popCount == 1)
		{
			// �� ȥ�� pop �ϳ�?

			// �̿� ȥ���ΰ� ���� ����� �ٸ� �����͵鵵 �����ϱ�.
			Node* node = _pendingList.exchange(nullptr);

			// ��������� �����س��� --�� �ص� ������ϰ� �����.
			if (--_popCount == 0)
			{
				// ����� �ְ� ���� -> ���� ����
				// �����ͼ� ����� ������ �����ʹ� �и��ص� ����~!
				DeleteNodes(node);
			}
			else if(node)
			{
				// ���� ���������� �ٽ� ���� ����.
				ChainPendingNodeList(node);
			}

			// �� �����ʹ� ����
			delete oldHead;
		}
		else
		{
			// ���� �ֳ�? �׷� ���� �������� �ʰ�, ���� ���ุ
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	// #2
	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;

		// �������� �����ٸ�?!
		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{

		}
	}

	// #2
	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		// last ã��
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
		// ����� ���� �� �����
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}


private:
	atomic<Node*> _head;


	// #2 �����Ǿ���� ����(ù��° ���)
	atomic<Node*> _pendingList;
	// #2 pop�� �������� ������ ����
	atomic<int32> _popCount = 0;
};