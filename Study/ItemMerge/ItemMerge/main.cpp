#include "pch.h"
#include "Item.h"
#include "ItemManager.h"

int main()
{
	ItemManager im;

	im.AddItem(0, 1, "단검", 1, EItemGrade::A);
	im.AddItem(1, 2, "단검", 2, EItemGrade::A);
	im.AddItem(2, 3, "갑옷", 1, EItemGrade::B);
	im.AddItem(3, 4, "반지", 2, EItemGrade::B);
	im.AddItem(4, 5, "반지", 3, EItemGrade::S);
	

	while (true)
	{
		if (im.GetArrIndex() == 0)
			break;

		cout << "강화할 템을 고르세요" << endl;

		int item1, item2;
		cin >> item1 >> item2;

		im.Show(item1, item2);
	}
}