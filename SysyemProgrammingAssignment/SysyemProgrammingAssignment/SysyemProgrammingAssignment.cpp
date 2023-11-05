#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void func1(string message, chrono::seconds delay)
{
    auto end = std::chrono::high_resolution_clock::now() + delay;
    while (chrono::high_resolution_clock::now() < end)
    {
        this_thread::yield();
    }
    cout << message << endl;
}

int main()
{
    thread t(func1, "Multi Thread", chrono::seconds(5));

    while (true)
    {
        func1("Main Thread", chrono::seconds(3));
    }

    t.join();

    return 0;
}