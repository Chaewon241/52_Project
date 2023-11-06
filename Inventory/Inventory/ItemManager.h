#pragma once

enum class ItemType;
class Item;

class ItemManager
{
public:
	ItemManager();
	~ItemManager();
public:
	// 아이템 추가
	void AddItem();
	
	// 아이템 삭제
	void DeleteItem();

	// 아이템 검색
	void SearchItem();
	
	// 아이템 정렬
	void SortItem();

	// 아이템 합성
	void MergeItem();

	// 아이템 목록 보여주기
	void ShowItem();

	ItemType NumToItemType(int num);
	//string ItemTypeToString(ItemType itemType);

private:
	vector<unique_ptr<Item>> m_ItemList;
	int m_ItemListIndex = 0;
};