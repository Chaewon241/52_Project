#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"

#include <iostream>

int main()
{
    int inputNum;
    unique_ptr<ItemManager> im = make_unique<ItemManager>();

    do
    {
        cout << endl;
        cout << "===== 메뉴 선택 =====" << endl;
        cout << "|| 1. 아이템 추가  ||" << endl;
        cout << "|| 2. 아이템 삭제  ||" << endl; 
        cout << "|| 3. 아이템 검색  ||" << endl; 
        cout << "|| 4. 아이템 목록  ||" << endl;
        cout << "|| 5. 아이템 정렬  ||" << endl;
        cout << "|| 6. 아이템 합성  ||" << endl;
        cout << "|| 0. 종료         ||" << endl;
        cout << "=====================" << endl;

        CheckInput(inputNum);

        cout << endl;
        if (inputNum == 1)
        {
            im->AddItem();
        }
        else if (inputNum == 2)
        {
            im->DeleteItem();
        }
        else if (inputNum == 3)
        {
            im->SearchItem();
        }
        else if (inputNum == 4)
        {
            im->ShowItem();
        }
        else if (inputNum == 5)
        {
            im->SortItem();
        }
        else if (inputNum == 6)
        {
            im->MergeItem();
        }
    } while (inputNum != 0);
}