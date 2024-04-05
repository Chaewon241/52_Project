#include "pch.h"
#include "CorePch.h"

#include <iostream>
#include <thread>

void HelloThread()
{
    cout << "Hello Thread" << endl;
}

int main()
{
    // 얘를 만들자마자 엔트리 포인트(실행할 함수)를 만들어줄 수 있음
    std::thread t(HelloThread);

    cout << "Hello Main" << endl;

    t.join();
}
