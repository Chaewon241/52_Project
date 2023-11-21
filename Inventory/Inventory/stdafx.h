#pragma once
#include <vector>
#include <string>
#include <memory>		// unique pointer
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

template <typename T>
T& CheckInput(T& t)
{
    // 문자일 때(A~D까지)
    if (typeid(t) == typeid(char))
    {
        cin >> t;
        if ((t >= 'a' && t <= 'd') || (t >= 'A' && t <= 'D') || (t == 's') || (t == 'S'))
        {
            return t;
        }
        else
        {
            cout << "다시 입력하세요" << endl;
            return CheckInput(t);
        }
    }


    while (!(cin >> t))
    {
        cerr << "잘못 입력하셨습니다." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "입력: ";
    }

    return t;
}