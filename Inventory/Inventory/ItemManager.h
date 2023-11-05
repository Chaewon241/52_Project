#pragma once

enum class ItemType;
class Item;

class ItemManager
{
public:
	ItemManager();
	~ItemManager();
public:
	void AddItem();
	void ShowItem();

private:
	vector<Item*> m_ItemList;
	int m_ItemListIndex = 0;
};