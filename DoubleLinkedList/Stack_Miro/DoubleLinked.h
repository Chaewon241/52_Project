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

	// 제일 앞에 추가
	void InsertAtHead(int value);
	
	// 제일 뒤에 추가
	void InsertAtTail(int value);
	
	// 노드값 전부 출력
	void PrintListAll();

	// 값으로 노드 찾기
	Node* FindNode(int value);

	// 노드 삭제하기
	void DeleteNode(Node* pNode);

	// 노드 삭제할 때 데이터가 같은 노드들을 전부 삭제
	void eraseNode(int value);
};

