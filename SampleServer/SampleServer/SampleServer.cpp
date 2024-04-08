#include "pch.h"
#include "CorePch.h"
#include "ConcurrentStack.h"
#include "ConcurrentQueue.h"

#include <iostream>
#include <thread>

LockQueue<int32> q;
LockFreeStack<int32> s;

void Push()
{
    while (true)
    {
        int32 value = rand() % 100;
        s.Push(value);
        this_thread::sleep_for(10ms);
    }
}

void Pop()
{
    while (true)
    {
        int32 data = 0;
        if (s.TryPop(data))
            cout << data << endl;
    }
}

int main()
{
    std::thread t1(Push);
    std::thread t2(Pop);
    std::thread t3(Pop);

    t1.join();
    t2.join();
    t3.join();
}
