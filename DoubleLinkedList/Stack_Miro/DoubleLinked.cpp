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

// 값으로 노드 찾기
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

// 노드 삭제하기
void DoubleLinked::DeleteNode(Node* pNode)
{
	Node* tmpData = nullptr;


	
	// 삭제하려는 노드가 Head, Tail 중간인 경우 모두 테스트 한다.
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

	// 중간
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