#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Ring.h"

/*
1. 아이템 목록에 아이템들이 들어있는지 확인하는 함수(o)
2. 등급 업 시킬 떄 연산자 오버로딩 다시 보기 (o)
3. 반복되는 부분 함수로 따로 빼기
4. 다른게 입력됐을 때 예외처리
5. 등급이 최고 등급일 때는 합성 안 하는거 예외처리 (o)
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

	cout << "아이템 타입을 입력하세요(1. 단검 2. 갑옷 3. 반지): ";
	int itemTypeNum;
	cin >> itemTypeNum;

	cout << "아이템 레벨을 입력하세요: ";
	int level;
	cin >> level;

	cout << "아이템 등급을 입력하세요(S, A, B, C, D): ";
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
	// grade enum class로 바꿔주기
	item->SetItemGrade(grade);
	item->SetItemLevel(level);
	
	m_ItemList.push_back(move(item));

	cout << "아이템이 추가되었습니다." << endl;
	
	ShowItem();
}

void ItemManager::DeleteItem()
{
	if (IsEmpty())
	{
		cout << "아이템 목록이 비어있습니다." << endl;
		return;
	}

	cout << "어떤 아이템을 삭제하시겠습니까? 1. 단검 2. 갑옷 3. 반지" << endl;
	// todo 아이템 삭제 전 아이템 목록 보여주기
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
		cout << "아이템 목록이 비어있습니다." << endl;
		return;
	}

	cout << "어떤 아이템을 검색하시겠습니까? 1. 단검 2. 갑옷 3. 반지" << endl;
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
		cout << "해당 아이템이 존재하지 않습니다." << endl;
}

void ItemManager::SortItem()
{
	if (IsEmpty())
	{
		cout << "아이템 목록이 비어있습니다." << endl;
		return;
	}

	cout << "어떤 기준으로 정렬하시겠습니까? 1. 이름 2. 등급 3. 레베루" << endl;
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

	// 여기서 show하고 다시 index로 sort 해주기.
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
		cout << "아이템 목록이 비어있습니다." << endl;
		return;
	}

	if (m_ItemList.size() == 1)
	{
		cout << "아이템이 하나밖에 없습니다." << endl;
		return;
	}

	cout << "합칠 아이템들의 인덱스를 하나씩 입력하세요." << endl;
	 
	// todo 아이템 합성하기 전 아이템 목록 보여주기
	ShowItem();

	int inputIndex1;
	int inputIndex2;

	cin >> inputIndex1;
	cin >> inputIndex2;

	if (inputIndex1 == inputIndex2)
	{
		cout << "둘의 인덱스가 같습니다." << endl;
		return;
	}

	int item1Index = -1;
	int item2Index = -1;

	for (int i = 0; i < m_ItemList.size(); i++)
	{
		// 첫번째 아이템 인덱스 저장해두기
		if (m_ItemList[i]->GetItemIndex() == inputIndex1)
		{
			item1Index = i;
			
		}
		// 두번째 아이템 인덱스 저장해두기
		else if (m_ItemList[i]->GetItemIndex() == inputIndex2)
		{
			item2Index = i;
		}
		// 둘다 저장 됐으면 break
		if (item1Index != -1 && item2Index != -1)
		{
			break;
		}
	}

	// 둘이 등급이 같으면 합병 진행
	if (m_ItemList[item1Index]->GetItemGrade() == m_ItemList[item2Index]->GetItemGrade())
	{
		if (m_ItemList[item1Index]->GetItemGrade() == GradeType::S)
		{
			cout << "이미 최고 등급" << endl;

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

		// 기존의 아이템들을 삭제
		// 삭제해주면서 인덱스가 변화하여 조건을 걸어줌.
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

		cout << "합성이 완료되었습니다." << endl;

		ShowItem();
	}
	else
	{
		cout << "둘의 등급이 다릅니다." << endl;
	}
}

void ItemManager::ShowItem()
{
	if (IsEmpty())
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
