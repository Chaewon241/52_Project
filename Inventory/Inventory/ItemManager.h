#pragma once

enum class ItemType;
enum class GradeType;

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

	bool IsEmpty();

	// int에서 itemtype으로
	ItemType NumToItemType(int num);
	//string ItemTypeToString(ItemType itemType);

	// GradeType에서 char로
	char GradeTypeToChar(GradeType& gradeType);

	// 위에거 반대
	GradeType CharToGradeType(char c);

private:
	vector<unique_ptr<Item>> m_ItemList;
	int m_ItemListIndex = 0;
};