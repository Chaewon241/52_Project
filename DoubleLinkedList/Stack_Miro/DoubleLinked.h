#pragma once

struct Node
{
	int data;
	Node* pNext = nullptr;
	Node* pPrev = nullptr;
};

struct DoubleLinked
{
	Node* pHead = nullptr;
	Node* pTail = nullptr;

	// ���� �տ� �߰�
	void InsertAtHead(int value);
	
	// ���� �ڿ� �߰�
	void InsertAtTail(int value);
	
	// ��尪 ���� ���
	void PrintListAll();

	// ������ ��� ã��
	Node* FindNode(int value);

	// ��� �����ϱ�
	void DeleteNode(Node* pNode);

	// ��� ������ �� �����Ͱ� ���� ������ ���� ����
	void eraseNode(int value);
};

