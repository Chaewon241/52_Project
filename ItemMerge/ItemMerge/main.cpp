#include "pch.h"
#include "Item.h"
#include "ItemManager.h"

int main()
{
	ItemManager itemIndex;
	Item* itemList = new Item[itemIndex.GetArrIndex()];

	ItemManager im;

	im.AddItem(itemList[0], 1, "´Ü°Ë", 1, EItemGrade::A);
	im.AddItem(itemList[1], 2, "´Ü°Ë", 2, EItemGrade::A);
	im.AddItem(itemList[2], 3, "°©¿Ê", 1, EItemGrade::B);
	im.AddItem(itemList[3], 4, "¹ÝÁö", 2, EItemGrade::B);
	im.AddItem(itemList[4], 5, "¹ÝÁö", 3, EItemGrade::S);

	im.Show(itemList, 2, 3);
	im.Show(itemList, 1, 2);
	im.Show(itemList, 3, 4);
}