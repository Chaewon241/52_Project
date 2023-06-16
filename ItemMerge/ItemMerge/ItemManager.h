#pragma once

using namespace std;

class Item;

class ItemManager
{
	int m_maxIndex;
	int m_arrIndex;

public:
    ItemManager()
        : m_maxIndex(0)
        , m_arrIndex(5)
    {
    }

    ~ItemManager()
    {
    }

    void SetArrIndex(int index) { m_arrIndex = index; }
	int GetArrIndex() { return m_arrIndex; }

    void SetMaxIndex(int index) { m_maxIndex = index; }
    int GetMaxIndex() { return m_maxIndex; }

	void AddItem(Item& item, int index, string name, int level, EItemGrade grade) {}

	void Show(Item* item, int item1, int item2)	{}

    void GradeUp(Item* item, int item1, int item2) {}

    char ConvertToChar(EItemGrade grade) {}
};
