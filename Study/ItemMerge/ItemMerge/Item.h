#pragma once

using namespace std;

enum class EItemGrade : int
{
	C = 0,
	B = 1,
	A = 2,
	S = 3,
	Max
};

class Item
{
private:
	int m_index;
	int m_level;
	string m_name;
	EItemGrade m_grade;

public:
	Item()
		: m_index(0)
		, m_level(0)
		, m_name()
		, m_grade()
	{
	}

	~Item() 
	{
	}

	void   SetIndex(int index) { m_index = index; }
	int    GetIndex() { return m_index; }

	void   SetName(string name) { m_name = name; }
	string GetName() { return m_name; }

	void   SetLevel(int level) { m_level = level; }
	int    GetLevel() { return m_level; }

	void   SetGrade(EItemGrade grade) { m_grade = grade; }
	EItemGrade   GetGrade() { return m_grade; }
};

