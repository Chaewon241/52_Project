#include "MyCircularQueue.h"

MyCircularQueue::MyCircularQueue(int maxQueueSize)
	:m_MaxQueueSize(maxQueueSize)
{
	m_Queue = new char*[maxQueueSize];
	m_Front = 0;
	m_Rear = 0;
}

MyCircularQueue::~MyCircularQueue()
{
	delete[] m_Queue;
}

bool MyCircularQueue::isEmpty() {
	if (m_Front == m_Rear)
	{
		return true;
	}
	return false;
}

bool MyCircularQueue::isFull() 
{
	if (m_Front == (m_Rear + 1) % m_MaxQueueSize)
	{
		return true;
	}
	return false;
}

void MyCircularQueue::enQueue(char* item) {
	if (isFull())
		return;
	else 
	{
		m_Queue[m_Rear] = item;
		m_Rear = ++m_Rear % m_MaxQueueSize;
	}
}

void MyCircularQueue::deQueue() {
	if (isEmpty())
		return;

	else 
		m_Front = ++m_Front % m_MaxQueueSize;
}

char* MyCircularQueue::Peek() const
{
	return m_Queue[m_Front % m_MaxQueueSize];
}
