#include <iostream>

#include "ItemManager.h"
#include "Item.h"

using namespace std;

void ItemManager::AddItem(int arrIndex, int index, string name, int level, EItemGrade grade)
{
    Item item;
    item.SetIndex(index);
    item.SetName(name);
    item.SetLevel(level);
    item.SetGrade(grade);

    m_itemList[arrIndex] = item;
    m_maxIndex++;
}

void ItemManager::Show(int item1, int item2)
{
	cout << "변경전" << endl;
	for (int i = 0; i < m_arrIndex + 1; i++)
	{
		cout << m_itemList[i].GetIndex() << ", " << m_itemList[i].GetName() << ", " << m_itemList[i].GetLevel() <<
			", " << ConvertToChar(m_itemList[i].GetGrade()) << endl;
	}
	cout << endl;

	cout << "변경후" << endl;
	GradeUp(item1, item2);
	for (int i = 0; i < m_arrIndex + 1; i++)
	{
		cout << m_itemList[i].GetIndex() << ", " << m_itemList[i].GetName() << ", " << m_itemList[i].GetLevel() <<
			", " << ConvertToChar(m_itemList[i].GetGrade()) << endl;
	}
    cout << endl;
}

void ItemManager::GradeUp(int item1, int item2)
{
    int itemIdx1 = -1;
    int itemIdx2 = -1;

    for (int i = 0; i <  m_arrIndex + 1; i++)
    {
        if (m_itemList[i].GetIndex() == item1)
        {
            itemIdx1 = i;
        }
        else if (m_itemList[i].GetIndex() == item2)
        {
            itemIdx2 = i;
        }

        if (itemIdx1 != -1 && itemIdx2 != -1)
        {
            break;
        }
    }

    if (m_itemList[itemIdx1].GetGrade() != m_itemList[itemIdx2].GetGrade())
    {
        cout << "강화 앙 실패띠~" << endl;
        cout << endl;
        return;
    }

    string itemName = m_itemList[itemIdx1].GetName();

    EItemGrade nextGrade = static_cast<EItemGrade>(static_cast<int>(m_itemList[itemIdx1].GetGrade()) + 1);

    int tmp = 0;

    for (int i = 0; i < m_arrIndex + 1; i++)
    {
        if (i == itemIdx1 || i == itemIdx2)
        {
            continue;
        }

        AddItem(tmp++, m_itemList[i].GetIndex(), m_itemList[i].GetName(), m_itemList[i].GetLevel(), m_itemList[i].GetGrade());
        m_maxIndex--;
    }

    m_arrIndex -= 1;
    m_maxIndex += 1;

    AddItem(m_arrIndex, m_maxIndex, itemName, 1, nextGrade);
    m_maxIndex--;
}

char ItemManager::ConvertToChar(EItemGrade grade)
{
    switch (grade)
    {
    case EItemGrade::C:
        return 'C';
    case EItemGrade::B:
        return 'B';
    case EItemGrade::A:
        return 'A';
    case EItemGrade::S:
        return 'S';
    default:
        return 'F';
    }
}