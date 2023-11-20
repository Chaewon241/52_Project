#pragma once
#include <vector>
#include <string>
#include <memory>		// unique pointer
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
T& CheckInput(T& t)
{
    if (typeid(t) == typeid(int))
    {
        while (!(cin >> t))
        {
            cerr << "올바르지 않은 입력입니다." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "다시 입력하세요: ";
        }
    }
    else if (typeid(t) == typeid(char))
    {
        while (cin.fail() || cin.peek() != '\n') 
        {
            cerr << "올바른 문자를 입력하세요." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "문자를 입력하세요: ";
        }
    }
    
    return t;
}