#pragma once

enum class ItemType;
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

	ItemType NumToItemType(int num);
	//string ItemTypeToString(ItemType itemType);

private:
	vector<unique_ptr<Item>> m_ItemList;
	int m_ItemListIndex = 0;
};