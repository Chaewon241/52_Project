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
            cerr << "�ùٸ��� ���� �Է��Դϴ�." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�ٽ� �Է��ϼ���: ";
        }
    }
    else if (typeid(t) == typeid(char))
    {
        while (cin.fail() || cin.peek() != '\n') 
        {
            cerr << "�ùٸ� ���ڸ� �Է��ϼ���." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "���ڸ� �Է��ϼ���: ";
        }
    }
    
    return t;
}