#pragma once

// namespace ¸¸µé±â
class MyCircularQueue 
{
private:
	char** m_Queue;
	int m_Front;
	int m_Rear;
	int m_MaxQueueSize;

public:
	MyCircularQueue(int size);
	~MyCircularQueue();
	bool isEmpty();
	bool isFull();
	void enQueue(char* item);
	void deQueue();
	char* Peek() const;
};