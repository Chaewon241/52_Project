#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Ring.h"

/*
1. ������ ��Ͽ� �����۵��� ����ִ��� Ȯ���ϴ� �Լ�(o)
2. ��� �� ��ų �� ������ �����ε� �ٽ� ���� (o)
3. �ݺ��Ǵ� �κ� �Լ��� ���� ����
4. �ٸ��� �Էµ��� �� ����ó��
5. ����� �ְ� ����� ���� �ռ� �� �ϴ°� ����ó�� (o)
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

	cout << "������ ������ �Է��ϼ���: ";
	int level;
	cin >> level;

	cout << "������ ����� �Է��ϼ���(S, A, B, C, D): ";
	char inputGrade;
	cin >> inputGrade;
	GradeType grade = CharToGradeType(inputGrade);

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
	if (IsEmpty())
	{
		cout << "������ ����� ����ֽ��ϴ�." << endl;
		return;
	}

	cout << "� �������� �����Ͻðڽ��ϱ�? 1. �ܰ� 2. ���� 3. ����" << endl;
	// todo ������ ���� �� ������ ��� �����ֱ�
	ShowItem();

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
	if (IsEmpty())
	{
		cout << "������ ����� ����ֽ��ϴ�." << endl;
		return;
	}

	cout << "� �������� �˻��Ͻðڽ��ϱ�? 1. �ܰ� 2. ���� 3. ����" << endl;
	int itemTypeNum;
	cin >> itemTypeNum;

	int itemCount = 0;

	if (NumToItemType(itemTypeNum) == ItemType::Weapon)
	{
		for (int i = 0; i < m_ItemList.size(); i++)
		{
			auto itemPtr = m_ItemList[i].get();
			if (dynamic_cast<Weapon*>(itemPtr) != nullptr)
			{
				itemCount++;
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
				itemCount++;
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
				itemCount++;
				cout << itemPtr->GetItemIndex() << ", " << itemPtr->GetItemName() << ", " << itemPtr->GetItemLevel()
					<< ", " << GradeTypeToChar(itemPtr->GetItemGrade()) << endl;
			}
		}
	}

	if (itemCount == 0)
		cout << "�ش� �������� �������� �ʽ��ϴ�." << endl;
}

void ItemManager::SortItem()
{
	if (IsEmpty())
	{
		cout << "������ ����� ����ֽ��ϴ�." << endl;
		return;
	}

	cout << "� �������� �����Ͻðڽ��ϱ�? 1. �̸� 2. ��� 3. ������" << endl;
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
	if (IsEmpty())
	{
		cout << "������ ����� ����ֽ��ϴ�." << endl;
		return;
	}

	if (m_ItemList.size() == 1)
	{
		cout << "�������� �ϳ��ۿ� �����ϴ�." << endl;
		return;
	}

	cout << "��ĥ �����۵��� �ε����� �ϳ��� �Է��ϼ���." << endl;
	 
	// todo ������ �ռ��ϱ� �� ������ ��� �����ֱ�
	ShowItem();

	int inputIndex1;
	int inputIndex2;

	cin >> inputIndex1;
	cin >> inputIndex2;

	if (inputIndex1 == inputIndex2)
	{
		cout << "���� �ε����� �����ϴ�." << endl;
		return;
	}

	int item1Index = -1;
	int item2Index = -1;

	for (int i = 0; i < m_ItemList.size(); i++)
	{
		// ù��° ������ �ε��� �����صα�
		if (m_ItemList[i]->GetItemIndex() == inputIndex1)
		{
			item1Index = i;
			
		}
		// �ι�° ������ �ε��� �����صα�
		else if (m_ItemList[i]->GetItemIndex() == inputIndex2)
		{
			item2Index = i;
		}
		// �Ѵ� ���� ������ break
		if (item1Index != -1 && item2Index != -1)
		{
			break;
		}
	}

	// ���� ����� ������ �պ� ����
	if (m_ItemList[item1Index]->GetItemGrade() == m_ItemList[item2Index]->GetItemGrade())
	{
		if (m_ItemList[item1Index]->GetItemGrade() == GradeType::S)
		{
			cout << "�̹� �ְ� ���" << endl;

			return;
		}
		m_ItemListIndex++;

		ItemPtr newItem;
		if (m_ItemList[item1Index]->GetItemType() == ItemType::Weapon)
		{
			WeaponPtr weaponItem = make_unique<Weapon>();
			newItem = move(weaponItem);
			newItem->SetItemType(ItemType::Weapon);
		}
		else if (m_ItemList[item1Index]->GetItemType() == ItemType::Armor)
		{
			ArmorPtr armorItem = make_unique<Armor>();
			newItem = move(armorItem);
			newItem->SetItemType(ItemType::Armor);
		}
		else if (m_ItemList[item1Index]->GetItemType() == ItemType::Ring)
		{
			RingPtr ringItem = make_unique<Ring>();
			newItem = move(ringItem);
			newItem->SetItemType(ItemType::Ring);
		}

		newItem->SetItemIndex(m_ItemListIndex);

		++(*m_ItemList[item1Index]);

		newItem->SetItemGrade(m_ItemList[item1Index]->GetItemGrade());
		newItem->SetItemLevel(1);
		m_ItemList.push_back(move(newItem));

		// ������ �����۵��� ����
		// �������ָ鼭 �ε����� ��ȭ�Ͽ� ������ �ɾ���.
		if (item1Index > item2Index)
		{
			m_ItemList.erase(m_ItemList.begin() + item2Index);
			m_ItemList.erase(m_ItemList.begin() + item1Index - 1);
		}
		else
		{
			m_ItemList.erase(m_ItemList.begin() + item1Index);
			m_ItemList.erase(m_ItemList.begin() + item2Index - 1);
		}

		sort(m_ItemList.begin(), m_ItemList.end(), [](const ItemPtr& a, const ItemPtr& b) {
			return a->GetItemIndex() < b->GetItemIndex();
			});

		cout << "�ռ��� �Ϸ�Ǿ����ϴ�." << endl;

		ShowItem();
	}
	else
	{
		cout << "���� ����� �ٸ��ϴ�." << endl;
	}
}

void ItemManager::ShowItem()
{
	if (IsEmpty())
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

bool ItemManager::IsEmpty()
{
	if (m_ItemList.size() > 0)
		return false;
	return true;
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
