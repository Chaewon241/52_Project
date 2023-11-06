#pragma once

enum class ItemType
{
	Weapon,
	Armor,
	Ring
};

class Item
{
public:
	Item();
	virtual ~Item();

public:
	void SetItemType(ItemType it) { m_ItemType = it; }
	void SetItemIndex(int index) { m_ItemIndex = index; }
	void SetItemName(string name) { m_ItemName = name; }
	void SetItemLevel(int level) { m_ItemLevel = level; }
	void SetItemGrade(char grade) { m_ItemGrade = grade; }

	ItemType GetItemType() { return m_ItemType; }
	int		 GetItemIndex() { return m_ItemIndex; }
	string	 GetItemName() { return m_ItemName; }
	int		 GetItemLevel() { return m_ItemLevel; }
	char	 GetItemGrade() { return m_ItemGrade; }

protected:
	ItemType	m_ItemType;
	int			m_ItemIndex;
	string		m_ItemName;
	int			m_ItemLevel;
	char		m_ItemGrade;
};

