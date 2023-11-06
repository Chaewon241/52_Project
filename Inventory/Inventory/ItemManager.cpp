#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Ring.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::AddItem()
{
	m_ItemListIndex++;

	// todo ���߿� �ٲܰ�) ������ ���� �Է� �ޱ�
	cout << "������ Ÿ��" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	cout << "������ ���" << endl;
	char grade;
	cin >> grade;

	cout << "������ ����" << endl;
	int level;
	cin >> level;

	unique_ptr<Item> item;
	
	if (NumToItemType(itemTypeNum) == ItemType::Weapon)
	{
		unique_ptr<Weapon> weaponItem = make_unique<Weapon>();
		item = std::move(weaponItem);
		item->SetItemType(ItemType::Weapon);
		item->SetItemName("�ܰ�");
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Armor)
	{
		unique_ptr<Armor> armorItem = make_unique<Armor>();
		item = std::move(armorItem);
		item->SetItemType(ItemType::Armor);
		item->SetItemName("����");
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Ring)
	{
		unique_ptr<Ring> ringItem = make_unique<Ring>();
		item = std::move(ringItem);
		item->SetItemType(ItemType::Ring);
		item->SetItemName("����");
	}

	item->SetItemIndex(m_ItemListIndex);
	item->SetItemGrade(grade);
	item->SetItemLevel(level);
	
	m_ItemList.push_back(std::move(item));
}

void ItemManager::DeleteItem()
{
	cout << "��� ����?" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	if (NumToItemType(itemTypeNum) == ItemType::Weapon)
	{
		for (int i = 0; i < m_ItemList.size(); i++)
		{
			auto itemPtr = m_ItemList[i].get();
			if (dynamic_cast<Weapon*>(itemPtr) != nullptr)
			{
				m_ItemList.erase(m_ItemList.begin() + i);
				i--;
			}
		}
	}

}

void ItemManager::SearchItem()
{

}

void ItemManager::SortItem()
{

}

void ItemManager::MergeItem()
{
}

void ItemManager::ShowItem()
{
	if (m_ItemList.size() == 0)
	{
		cout << "������ ����� ����ֽ��ϴ�." << endl;
		return;
	}

	for (const auto& itemList : m_ItemList)
	{
		cout << itemList->GetItemIndex() << ", " << itemList->GetItemName() << ", " << itemList->GetItemLevel()
			<< ", "  << itemList->GetItemGrade() << endl;
	}
}

ItemType ItemManager::NumToItemType(int num)
{
	if (num == 1)
		return ItemType::Weapon;
	else if (num == 2)
		return ItemType::Armor;
	else if (num == 3)
		return ItemType::Ring;
}

//string ItemManager::ItemTypeToString(ItemType itemType)
//{
//	if (itemType == ItemType::Weapon)
//		return "Weapon";
//	else if (itemType == ItemType::Armor)
//		return ItemType::Armor;
//	else if (itemType == ItemType::Ring)
//		return ItemType::Ring;
//}
