#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"
#include "Weapon.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::AddItem()
{
	m_ItemListIndex++;

	// 아이템 정보 입력 받기
	cout << "아이템 타입" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	cout << "아이템 이름" << endl;
	string name;
	cin >> name;

	cout << "아이템 등급" << endl;
	char grade;
	cin >> grade;

	cout << "아이템 레벨" << endl;
	int level;
	cin >> level;

	Item* item = nullptr;

	if (itemTypeNum == 1)
	{
		item = new Weapon;
		item->SetItemType(ItemType::Weapon);
	}
	/*else if (itemType == ItemType::Armor)
	{
		unique_ptr<Item> weapon = make_unique<Weapon>(name);
		m_ItemList.push_back(weapon);
	}
	else if (itemType == ItemType::Ring)
	{
		unique_ptr<Item> weapon = make_unique<Weapon>(name);
		m_ItemList.push_back(weapon);
	}*/

	item->SetItemIndex(m_ItemListIndex);
	item->SetItemGrade(grade);
	item->SetItemLevel(level);
	item->SetItemName(name);
	m_ItemList.push_back(item);
}

void ItemManager::ShowItem()
{

	for (const auto& itemList : m_ItemList)
	{
		cout << itemList->GetItemGrade() << ", " << itemList->GetItemName()
			<< ", " << itemList->GetItemLevel() << ", " << itemList->GetItemGrade() << endl;
	}
}
