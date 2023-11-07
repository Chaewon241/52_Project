#pragma once

enum class ItemType
{
	Weapon,
	Armor,
	Ring
};

enum class GradeType
{
	D,
	C,
	B,
	A,
	S,
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
	void SetItemGrade(GradeType grade) { m_ItemGrade = grade; }

	ItemType& GetItemType() { return m_ItemType; }
	int&		 GetItemIndex() { return m_ItemIndex; }
	string&	 GetItemName() { return m_ItemName; }
	int&		 GetItemLevel() { return m_ItemLevel; }
	GradeType&	 GetItemGrade() { return m_ItemGrade; }

	Item& operator++()
	{
		int gradeInt = (int)m_ItemGrade;
		gradeInt++;
		m_ItemGrade = (GradeType)gradeInt;

		return *this;
	}

protected:
	ItemType	m_ItemType;
	int			m_ItemIndex;
	string		m_ItemName;
	int			m_ItemLevel;
	GradeType	m_ItemGrade;
};

using ItemPtr = unique_ptr<Item>;