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

	// todo 나중에 바꿀거) 아이템 정보 입력 받기
	cout << "아이템 타입" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	cout << "아이템 등급" << endl;
	char tempGrade;
	cin >> tempGrade;

	GradeType grade = CharToGradeType(tempGrade);

	cout << "아이템 레벨" << endl;
	int level;
	cin >> level;

	unique_ptr<Item> item;
	
	if (NumToItemType(itemTypeNum) == ItemType::Weapon)
	{
		unique_ptr<Weapon> weaponItem = make_unique<Weapon>();
		item = move(weaponItem);
		item->SetItemType(ItemType::Weapon);
		// setname을 여기서 해주지 말고 하위 아이템들에서 해주자.
		item->SetItemName("단검");
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Armor)
	{
		unique_ptr<Armor> armorItem = make_unique<Armor>();
		item = move(armorItem);
		item->SetItemType(ItemType::Armor);
		item->SetItemName("갑옷");
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Ring)
	{
		unique_ptr<Ring> ringItem = make_unique<Ring>();
		item = move(ringItem);
		item->SetItemType(ItemType::Ring);
		item->SetItemName("반지");
	}

	item->SetItemIndex(m_ItemListIndex);
	// grade enum class로 바꿔주기
	item->SetItemGrade(grade);
	item->SetItemLevel(level);
	
	m_ItemList.push_back(move(item));
}

void ItemManager::DeleteItem()
{
	cout << "어떤거 삭제?" << endl;
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
	else if (NumToItemType(itemTypeNum) == ItemType::Armor)
	{
		for (int i = 0; i < m_ItemList.size(); i++)
		{
			auto itemPtr = m_ItemList[i].get();
			if (dynamic_cast<Armor*>(itemPtr) != nullptr)
			{
				m_ItemList.erase(m_ItemList.begin() + i);
				i--;
			}
		}
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Ring)
	{
		for (int i = 0; i < m_ItemList.size(); i++)
		{
			auto itemPtr = m_ItemList[i].get();
			if (dynamic_cast<Armor*>(itemPtr) != nullptr)
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
	cout << "어떻게 정렬? 1. 이름 2. 등급 3. 레베루" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	if (itemTypeNum == 1)
	{
		sort(m_ItemList.begin(), m_ItemList.end(), [](const unique_ptr<Item>& a, const unique_ptr<Item>& b) {
			return a->GetItemName() < b->GetItemName();
		});
	}
	else if (itemTypeNum == 2)
	{
		sort(m_ItemList.begin(), m_ItemList.end(), [](const unique_ptr<Item>& a, const unique_ptr<Item>& b) {
			return a->GetItemGrade() < b->GetItemGrade();
			});
	}
	else if (itemTypeNum == 3)
	{
		sort(m_ItemList.begin(), m_ItemList.end(), [](const unique_ptr<Item>& a, const unique_ptr<Item>& b) {
			return a->GetItemLevel() < b->GetItemLevel();
			});
	}

	// 여기서 show하고 다시 index로 sort 해주기.
	ShowItem();

	cout << endl;

	sort(m_ItemList.begin(), m_ItemList.end(), [](const unique_ptr<Item>& a, const unique_ptr<Item>& b) {
		return a->GetItemIndex() < b->GetItemIndex();
		});

}

void ItemManager::MergeItem()
{
	cout << "어떤 아이템 합쳐" << endl;
	int itemIndex1;
	int itemIndex2;
	cin >> itemIndex1;
	cin >> itemIndex2;

	unique_ptr<Item> item1;
	unique_ptr<Item> item2;

	for (int i = 0; i < m_ItemList.size(); i++)
	{
		if (m_ItemList[i]->GetItemIndex() == itemIndex1)
		{
			item1 = move(m_ItemList[i]);
			m_ItemList.erase(m_ItemList.begin() + i);
			i--;

		}
		else if (m_ItemList[i]->GetItemIndex() == itemIndex2)
		{
			item2 = move(m_ItemList[i]);
			m_ItemList.erase(m_ItemList.begin() + i);
			i--;
		}

		if (item1 != nullptr && item2 != nullptr)
			break;
	}

	if (item1->GetItemGrade() == item2->GetItemGrade())
	{
		m_ItemListIndex++;
		unique_ptr<Item> newItem;
		if (item1->GetItemType() == ItemType::Weapon)
		{
			unique_ptr<Weapon> weaponItem = make_unique<Weapon>();
			newItem = move(weaponItem);
			newItem->SetItemType(ItemType::Weapon);
			// setname을 여기서 해주지 말고 하위 아이템들에서 해주자.
			newItem->SetItemName("단검");
		}
		else if (item1->GetItemType() == ItemType::Armor)
		{
			unique_ptr<Armor> armorItem = make_unique<Armor>();
			newItem = move(armorItem);
			newItem->SetItemType(ItemType::Armor);
			newItem->SetItemName("갑옷");
		}
		else if (item1->GetItemType() == ItemType::Ring)
		{
			unique_ptr<Ring> ringItem = make_unique<Ring>();
			newItem = move(ringItem);
			newItem->SetItemType(ItemType::Ring);
			newItem->SetItemName("반지");
		}

		newItem->SetItemIndex(m_ItemListIndex);

		item1->operator++();

		newItem->SetItemGrade(item1->GetItemGrade());
		newItem->SetItemLevel(1);
		m_ItemList.push_back(move(newItem));
	}
	else
	{
		cout << "둘이 다름" << endl;
		m_ItemList.push_back(move(item1));
		m_ItemList.push_back(move(item2));
	}
	
	sort(m_ItemList.begin(), m_ItemList.end(), [](const unique_ptr<Item>& a, const unique_ptr<Item>& b) {
		return a->GetItemIndex() < b->GetItemIndex();
		});
}

void ItemManager::ShowItem()
{
	if (m_ItemList.size() == 0)
	{
		cout << "아이템 목록이 비어있습니다." << endl;
		return;
	}

	for (const auto& itemList : m_ItemList)
	{
		cout << itemList->GetItemIndex() << ", " << itemList->GetItemName() << ", " << itemList->GetItemLevel()
			<< ", "  << GradeTypeToChar(itemList->GetItemGrade()) << endl;
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

char ItemManager::GradeTypeToChar(GradeType& gradeType)
{
	if (gradeType == GradeType::S)
		return 'S';
	else if (gradeType == GradeType::A)
		return 'A';
	else if (gradeType == GradeType::B)
		return 'B';
	else if (gradeType == GradeType::C)
		return 'C';
	else if (gradeType == GradeType::D)
		return 'D';
}

GradeType ItemManager::CharToGradeType(char c)
{
	if (c == 'S')
		return GradeType::S;
	else if (c == 'A')
		return GradeType::A;
	else if (c == 'B')
		return GradeType::B;
	else if (c == 'C')
		return GradeType::C;
	else if (c == 'D')
		return GradeType::D;
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
