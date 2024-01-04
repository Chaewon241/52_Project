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
	// ������ �߰�
	void AddItem();
	
	// ������ ����
	void DeleteItem();

	// ������ �˻�
	void SearchItem();
	
	// ������ ����
	void SortItem();

	// ������ �ռ�
	void MergeItem();

	// ������ ��� �����ֱ�
	void ShowItem();

	bool IsEmpty();

	// int���� itemtype����
	ItemType NumToItemType(int num);
	//string ItemTypeToString(ItemType itemType);

	// GradeType���� char��
	char GradeTypeToChar(GradeType& gradeType);

	// ������ �ݴ�
	GradeType CharToGradeType(char c);

private:
	vector<unique_ptr<Item>> m_ItemList;
	int m_ItemListIndex = 0;
};