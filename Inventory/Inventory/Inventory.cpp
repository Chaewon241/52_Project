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
        cin >> inputNum;
        if (inputNum == 1)
        {
            im->AddItem();
        }
    }
}

