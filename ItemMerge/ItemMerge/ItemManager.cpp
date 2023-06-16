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

	cout << "������" << endl;
	for (int i = 0; i < im->GetArrIndex() + 1; i++)
	{
		cout << item[i].GetIndex() << ", " << item[i].GetName() << ", " << item[i].GetLevel() <<
			", " << ConvertToChar(item[i].GetGrade()) << endl;
	}
	cout << endl;

	cout << "������" << endl;
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

    // ������ ������ Ŭ������ m_arrIndex�� ������ �־ �� �� �ִµ�,
    // �迭�� �ּҰ����θ� �Լ� ���� �Ҷ��� �迭�� ������ ������ ���� ������ָ鼭 �Ѱܾ�
    // �迭�� �ʰ��ϴ� �ּҰ��� ���� ���� ����

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
        cout << "��ȭ �� ���ж�~" << endl;
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