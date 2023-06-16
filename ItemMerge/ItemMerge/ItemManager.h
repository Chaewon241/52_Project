#pragma once
#include <string>
#include "Item.h"

using namespace std;

class Item;

class ItemManager
{
private:
	int m_maxIndex;
	int m_arrIndex;
    Item* m_itemList;
public:
    ItemManager()
        : m_maxIndex(0)
        , m_arrIndex(4)
    {
        m_itemList = new Item[m_arrIndex + 1];
    }

    ~ItemManager()
    {
    }

    Item* GetItemList() { return m_itemList; }

    void SetArrIndex(int index) { m_arrIndex = index; }
	int GetArrIndex() { return m_arrIndex; }

    void SetMaxIndex(int index) { m_maxIndex = index; }
    int GetMaxIndex() { return m_maxIndex; }

    void AddItem(int arrIndex, int index, string name, int level, EItemGrade grade);

    void Show(int item1, int item2);

    void GradeUp(int item1, int item2);

    char ConvertToChar(EItemGrade grade);
};
