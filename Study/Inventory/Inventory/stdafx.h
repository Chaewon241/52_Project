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
    // ������ ��(A~D����)
    if (typeid(t) == typeid(char))
    {
        cin >> t;
        if ((t >= 'a' && t <= 'd') || (t >= 'A' && t <= 'D') || (t == 's') || (t == 'S'))
        {
            return t;
        }
        else
        {
            cout << "�ٽ� �Է��ϼ���" << endl;
            return CheckInput(t);
        }
    }


    while (!(cin >> t))
    {
        cerr << "�߸� �Է��ϼ̽��ϴ�." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "�Է�: ";
    }

    return t;
}