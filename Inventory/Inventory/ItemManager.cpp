#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Ring.h"

/*
1. ������ ��Ͽ� �����۵��� ����ִ��� Ȯ���ϴ� �Լ�
2. ��� �� ��ų �� ������ �����ε� �ٽ� ����
3. �ݺ��Ǵ� �κ� �Լ��� ���� ����
4. �ٸ��� �Էµ��� �� ����ó��
5. ����� �ְ� ����� ���� �ռ� �� �ϴ°� ����ó��
*/

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::AddItem()
{
	m_ItemListIndex++;

	cout << "������ Ÿ���� �Է��ϼ���(1. �ܰ� 2. ���� 3. ����): ";
	int itemTypeNum;
	cin >> itemTypeNum;

	cout << "������ ����� �Է��ϼ���(S, A, B, C, D): ";
	char tempGrade;
	cin >> tempGrade;
	GradeType grade = CharToGradeType(tempGrade);

	cout << "������ ������ �Է��ϼ���: ";
	int level;
	cin >> level;

	ItemPtr item;
	
	if (NumToItemType(itemTypeNum) == ItemType::Weapon)
	{
		WeaponPtr weaponItem = make_unique<Weapon>();
		item = move(weaponItem);
		item->SetItemType(ItemType::Weapon);
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Armor)
	{
		ArmorPtr armorItem = make_unique<Armor>();
		item = move(armorItem);
		item->SetItemType(ItemType::Armor);
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Ring)
	{
		RingPtr ringItem = make_unique<Ring>();
		item = move(ringItem);
		item->SetItemType(ItemType::Ring);
	}

	item->SetItemIndex(m_ItemListIndex);
	// grade enum class�� �ٲ��ֱ�
	item->SetItemGrade(grade);
	item->SetItemLevel(level);
	
	m_ItemList.push_back(move(item));

	cout << "�������� �߰��Ǿ����ϴ�." << endl;
	
	ShowItem();
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
			if (dynamic_cast<Ring*>(itemPtr) != nullptr)
			{
				m_ItemList.erase(m_ItemList.begin() + i);
				i--;
			}
		}
	}
}

void ItemManager::SearchItem()
{
	cout << "��� �˻�? 1. �ܰ� 2. ���� 3. ����" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	if (NumToItemType(itemTypeNum) == ItemType::Weapon)
	{
		for (int i = 0; i < m_ItemList.size(); i++)
		{
			auto itemPtr = m_ItemList[i].get();
			if (dynamic_cast<Weapon*>(itemPtr) != nullptr)
			{
				cout << itemPtr->GetItemIndex() << ", " << itemPtr->GetItemName() << ", " << itemPtr->GetItemLevel()
					<< ", " << GradeTypeToChar(itemPtr->GetItemGrade()) << endl;
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
				cout << itemPtr->GetItemIndex() << ", " << itemPtr->GetItemName() << ", " << itemPtr->GetItemLevel()
					<< ", " << GradeTypeToChar(itemPtr->GetItemGrade()) << endl;
			}
		}
	}
	else if (NumToItemType(itemTypeNum) == ItemType::Ring)
	{
		for (int i = 0; i < m_ItemList.size(); i++)
		{
			auto itemPtr = m_ItemList[i].get();
			if (dynamic_cast<Ring*>(itemPtr) != nullptr)
			{
				cout << itemPtr->GetItemIndex() << ", " << itemPtr->GetItemName() << ", " << itemPtr->GetItemLevel()
					<< ", " << GradeTypeToChar(itemPtr->GetItemGrade()) << endl;
			}
		}
	}
}

void ItemManager::SortItem()
{
	cout << "��� ����? 1. �̸� 2. ��� 3. ������" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	if (itemTypeNum == 1)
	{
		sort(m_ItemList.begin(), m_ItemList.end(), [](const ItemPtr& a, const ItemPtr& b) {
			return a->GetItemName() < b->GetItemName();
		});
	}
	else if (itemTypeNum == 2)
	{
		sort(m_ItemList.begin(), m_ItemList.end(), [](const ItemPtr& a, const ItemPtr& b) {
			return a->GetItemGrade() < b->GetItemGrade();
			});
	}
	else if (itemTypeNum == 3)
	{
		sort(m_ItemList.begin(), m_ItemList.end(), [](const ItemPtr& a, const ItemPtr& b) {
			return a->GetItemLevel() < b->GetItemLevel();
			});
	}

	// ���⼭ show�ϰ� �ٽ� index�� sort ���ֱ�.
	ShowItem();

	cout << endl;

	sort(m_ItemList.begin(), m_ItemList.end(), [](const ItemPtr& a, const ItemPtr& b) {
		return a->GetItemIndex() < b->GetItemIndex();
		});

}

void ItemManager::MergeItem()
{
	cout << "� ������ ����" << endl;
	 
	int itemIndex1;
	int itemIndex2;

	cin >> itemIndex1;
	cin >> itemIndex2;

	ItemPtr item1;
	ItemPtr item2;

	int item1Index;
	int item2Index;

	for (int i = 0; i < m_ItemList.size(); i++)
	{
		if (m_ItemList[i]->GetItemIndex() == itemIndex1)
		{
			item1 = move(m_ItemList[i]);
			item1Index = i;
			
		}
		else if (m_ItemList[i]->GetItemIndex() == itemIndex2)
		{
			item2 = move(m_ItemList[i]);
			item2Index = i;
		}

		if (item1 != nullptr && item2 != nullptr)
		{
			break;
		}
	}

	if (item1->GetItemGrade() == item2->GetItemGrade())
	{
		if (item1->GetItemGrade() == GradeType::S)
		{
			cout << "�̹� �ְ� ���" << endl;

			m_ItemList[item1Index] = move(item1);
			m_ItemList[item2Index] = move(item2);

			return;
		}

		m_ItemListIndex++;
		ItemPtr newItem;
		if (item1->GetItemType() == ItemType::Weapon)
		{
			WeaponPtr weaponItem = make_unique<Weapon>();
			newItem = move(weaponItem);
			newItem->SetItemType(ItemType::Weapon);
		}
		else if (item1->GetItemType() == ItemType::Armor)
		{
			ArmorPtr armorItem = make_unique<Armor>();
			newItem = move(armorItem);
			newItem->SetItemType(ItemType::Armor);
		}
		else if (item1->GetItemType() == ItemType::Ring)
		{
			RingPtr ringItem = make_unique<Ring>();
			newItem = move(ringItem);
			newItem->SetItemType(ItemType::Ring);
		}

		newItem->SetItemIndex(m_ItemListIndex);

		++(*item1);

		newItem->SetItemGrade(item1->GetItemGrade());
		newItem->SetItemLevel(1);
		m_ItemList.push_back(move(newItem));
	}
	else
	{
		cout << "���� �ٸ�" << endl;
		m_ItemList.push_back(move(item1));
		m_ItemList.push_back(move(item2));
	}

	m_ItemList.erase(m_ItemList.begin() + item1Index);
	m_ItemList.erase(m_ItemList.begin() + item2Index);
	
	sort(m_ItemList.begin(), m_ItemList.end(), [](const ItemPtr& a, const ItemPtr& b) {
		return a->GetItemIndex() < b->GetItemIndex();
		});
}

void ItemManager::ShowItem()
{
	// ������ ��� ����ִ��� Ȯ���ϴ� �Լ��� ���� �־���ҵ�
	if (m_ItemList.size() == 0)
	{
		cout << "������ ����� ����ֽ��ϴ�." << endl;
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
	if (c == 'S' || c == 's')
		return GradeType::S;
	else if (c == 'A' || c == 'a')
		return GradeType::A;
	else if (c == 'B' || c == 'b')
		return GradeType::B;
	else if (c == 'C' || c == 'c')
		return GradeType::C;
	else if (c == 'D' || c == 'd')
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
