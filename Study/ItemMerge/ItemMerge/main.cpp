#include "pch.h"
#include "Item.h"
#include "ItemManager.h"

int main()
{
	ItemManager im;

	im.AddItem(0, 1, "�ܰ�", 1, EItemGrade::A);
	im.AddItem(1, 2, "�ܰ�", 2, EItemGrade::A);
	im.AddItem(2, 3, "����", 1, EItemGrade::B);
	im.AddItem(3, 4, "����", 2, EItemGrade::B);
	im.AddItem(4, 5, "����", 3, EItemGrade::S);
	

	while (true)
	{
		if (im.GetArrIndex() == 0)
			break;

		cout << "��ȭ�� ���� ������" << endl;

		int item1, item2;
		cin >> item1 >> item2;

		im.Show(item1, item2);
	}
}