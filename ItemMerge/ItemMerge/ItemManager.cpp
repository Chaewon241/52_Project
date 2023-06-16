#include "ItemManager.h"
#include "Item.h"
#include "pch.h"

using namespace std;

void AddItem(Item& item, int index, string name, int level, EItemGrade grade)
{
	item.SetIndex(index);
	item.SetName(name);
	item.SetGrade(grade);
}

void Show(Item* item, int item1, int item2)
{
	ItemManager* im;

	cout << "변경전" << endl;
	for (int i = 0; i < im->GetArrIndex() + 1; i++)
	{
		cout << item[i].GetIndex() << ", " << item[i].GetName() << ", " << item[i].GetLevel() <<
			", " << ConvertToChar(item[i].GetGrade()) << endl;
	}
	cout << endl;

	cout << "변경후" << endl;
	im->GradeUp(item, item1, item2);
	for (int i = 0; i < im->GetArrIndex() + 1; i++)
	{
		cout << item[i].GetIndex() << ", " << item[i].GetName() << ", " << item[i].GetLevel() <<
			", " << ConvertToChar(item[i].GetGrade()) << endl;
	}
}

void GradeUp(Item* item, int item1, int item2)
{
    ItemManager* im;

    if (!item)
        return;

    int itemIdx1 = -1;
    int itemIdx2 = -1;

    // 지금은 아이템 클래스가 m_arrIndex를 가지고 있어서 알 수 있는데,
    // 배열을 주소값으로만 함수 전달 할때는 배열이 끝나는 지점도 같이 명시해주면서 넘겨야
    // 배열을 초과하는 주소값을 접근 하지 않음

    for (int i = 0; i < im->GetArrIndex() + 1; i++)
    {
        if (item[i].GetIndex() == item1)
        {
            itemIdx1 = i;
        }
        else if (item[i].GetIndex() == item2)
        {
            itemIdx2 = i;
        }

        if (itemIdx1 != -1 && itemIdx2 != -1)
        {
            break;
        }
    }

    if (item[itemIdx1].GetGrade() != item[itemIdx2].GetGrade())
    {
        cout << "강화 앙 실패띠~" << endl;
        cout << endl;
        return;
    }

    string itemName = item[itemIdx1].GetName();

    EItemGrade nextGrade = static_cast<EItemGrade>(static_cast<int>(item[itemIdx1].GetGrade()) + 1);

    int tmp = 0;

    for (int i = 0; i < im->GetArrIndex() + 1; i++)
    {
        if (i == itemIdx1 || i == itemIdx2)
        {
            continue;
        }

        AddItem(item[tmp++], item[i].GetIndex(), item[i].GetName(), item[i].GetLevel(), item[i].GetGrade());
    }

    im->SetArrIndex(im->GetArrIndex() - 1);
    im->SetMaxIndex(im->GetMaxIndex() - 1);

    AddItem(item[im->GetArrIndex()], im->GetArrIndex(), itemName, 1, nextGrade);
}

char ConvertToChar(EItemGrade grade) 
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
    }
}