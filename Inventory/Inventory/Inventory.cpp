#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"

#include <iostream>

int main()
{
    int inputNum = -1;
    unique_ptr<ItemManager> im = make_unique<ItemManager>();

    while (inputNum != 0)
    {
        cout << "메뉴 선택" << endl;
        cin >> inputNum;

        if (inputNum == 1)
        {
            cout << "아이템 추가" << endl;
            im->AddItem();
        }
        else if (inputNum == 2)
        {
            im->ShowItem();
        }
        else if (inputNum == 3)
        {
            im->DeleteItem();
        }
    }
}

