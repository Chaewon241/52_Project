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
        auto data = s.TryPop();
        if (data)
        {
            cout << (*data) << endl;
        }
    }
}

int main()
{
    shared_ptr<int32> ptr;
    //bool value = ;


    std::thread t1(Push);
    std::thread t2(Pop);
    std::thread t3(Pop);

    t1.join();
    t2.join();
    t3.join();
}
