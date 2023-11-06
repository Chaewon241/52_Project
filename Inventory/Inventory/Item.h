#pragma once

enum class ItemType
{
	Weapon,
	Armor,
	Ring
};

enum class GradeType
{
	S,
	A,
	B,
	C,
	D,
	END
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

	GradeType& operator++()
	{
		if (m_ItemGrade == GradeType::S)
		{
			m_ItemGrade = GradeType::END;
		}
		else if (m_ItemGrade == GradeType::A)
		{
			m_ItemGrade = GradeType::S;
		}
		else if (m_ItemGrade == GradeType::B)
		{
			m_ItemGrade = GradeType::A;
		}
		else if (m_ItemGrade == GradeType::C)
		{
			m_ItemGrade = GradeType::B;
		}
		else if (m_ItemGrade == GradeType::D)
		{
			m_ItemGrade = GradeType::C;
		}
		return m_ItemGrade;
	}

protected:
	ItemType	m_ItemType;
	int			m_ItemIndex;
	string		m_ItemName;
	int			m_ItemLevel;
	GradeType	m_ItemGrade;
};
