#include "DoubleLinked.h"
#include <iostream>

using namespace std;

void DoubleLinked::InsertAtHead(int value)
{
	Node* pNewNode = new Node;
	pNewNode->data = value;

	if (pHead == nullptr)
	{
		pHead = pNewNode;
		pTail = pNewNode;
		return;
	}

	pHead->pPrev = pNewNode;
	pNewNode->pNext = pHead;
	pHead = pNewNode;
}

void DoubleLinked::InsertAtTail(int value)
{
	Node* pNewNode = new Node;
	pNewNode->data = value;

	if (pTail == nullptr)
	{
		pHead = pNewNode;
		pTail = pNewNode;
		return;
	}

	pTail->pNext = pNewNode;
	pNewNode->pPrev = pTail;
	pTail = pNewNode;
}

void DoubleLinked::PrintListAll()
{
	Node* pNode = pHead;
	while (pNode != nullptr)
	{
		cout << pNode->data << " ";
		pNode = pNode->pNext;
	}
	cout << endl;
}

// ������ ��� ã��
Node* DoubleLinked::FindNode(int value)
{
	Node* result = nullptr;
	Node* pNode = pHead;

	while (pNode != nullptr)
	{
		if (pNode->data == value) {
			result = pNode;
			return result;
		}
		pNode = pNode->pNext;
	}
	return result;
}

// ��� �����ϱ�
void DoubleLinked::DeleteNode(Node* pNode)
{
	Node* tmpData = nullptr;


	
	// �����Ϸ��� ��尡 Head, Tail �߰��� ��� ��� �׽�Ʈ �Ѵ�.
	if (pNode == pHead)
	{
		tmpData = pHead->pNext;
		delete pHead;
		pHead = tmpData;
		pHead->pNext = tmpData->pNext;
		pHead->pPrev = nullptr;
	}
		
	else if (pNode == pTail)
	{
		tmpData = pTail->pPrev;
		delete pTail;
		pTail = tmpData;
		pTail->pPrev = tmpData->pPrev;
		pTail->pNext = nullptr;
	}

	// �߰�
	else
	{
		Node* tmpData1 = pNode->pPrev;
		tmpData = pNode->pNext;
		tmpData->pPrev = tmpData1;
		tmpData1->pNext = tmpData;
		delete pNode;
	}
}

void DoubleLinked::eraseNode(int value)
{
	Node* pNode = pHead;

	while (pNode != nullptr)
	{
		if (pNode->data == value) 
		{
			if (pNode->pNext == nullptr)
			{
				DeleteNode(pNode);
				return;
			}
			pNode = pNode->pNext;
			DeleteNode(pNode->pPrev);
			continue;
		}
		pNode = pNode->pNext;
	}
}