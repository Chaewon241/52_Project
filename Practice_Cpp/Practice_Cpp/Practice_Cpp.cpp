#include <iostream>

using namespace std;

class Item
{
private:
	int index = 1, maxIndex = 5, arrIndex = 4;
	int level;
	string name;
	char grade;

public:
	void SetIndex(int index) { this->index = index; }
	int GetIndex()	{ return this->index; }
	int GetArrIndex()	{ return this->arrIndex; }

	void SetName(string name) {	this->name = name; }
	string GetName() { return this->name; }

	void SetLevel(int level) { this->level = level; }
	int GetLevel()	{ return this->level; }

	void SetGrade(char grade)	{ this->grade = grade; }
	char GetGrade()	{ return this->grade; }
	
	void AddItem(Item& item, int index, string name, int level, char grade)
	{
		item.SetIndex(index);
		item.SetName(name);
		item.SetLevel(level);
		item.SetGrade(grade);
	}

	void PrintList(Item* item)
	{
		for (int i = 0; i < arrIndex + 1; i++)
		{
			cout << item[i].GetIndex() << ", " << item[i].GetName() << ", " << item[i].GetLevel() <<
				", " << item[i].GetGrade() << endl;
		}
		cout << endl;
	}

	void Show(Item* item, int n1, int n2)
	{
		cout << "변경 전" << endl;
		PrintList(item);
		cout << "변경 후" << endl;
		MergeItem(item, n1, n2);
	}

	char GradeUp(char grade)
	{
		switch (grade)
		{
		case 'A':
			return 'S';
		case 'B':
			return 'A';
		case 'C':
			return 'B';
		default:
			break;
		}
	}

	void MergeItem(Item* item, int item1, int item2)
	{
		int itemidx1 = -1;
		int itemidx2 = -1;

		for (int i = 0; i < arrIndex + 1; i++)
		{
			if (item[i].GetIndex() == item1)
			{
				itemidx1 = i;
			}
			else if (item[i].GetIndex() == item2)
			{
				itemidx2 = i;
			}

			if (itemidx1 != -1 && itemidx2 != -1)
			{
				break;
			}
		}

		if (item[itemidx1].GetGrade() == item[itemidx2].GetGrade())
		{
			string itemname = item[itemidx1].GetName();
			char grade = GradeUp(item[itemidx1].GetGrade());
			int tmp = 0;

			for (int i = 0; i < arrIndex + 1; i++)
			{
				if (i == itemidx1 || i == itemidx2)
				{
					continue;
				}
				AddItem(item[tmp++], item[i].GetIndex(), item[i].GetName(), item[i].GetLevel(), item[i].GetGrade());
			}

			AddItem(item[--arrIndex], ++maxIndex, itemname, 1, grade);
			
			PrintList(item);
		}
		else
		{
			cout << "강화 실패띠~" << endl;
			cout << endl;
		}
	}
};

int main(void)
{
	Item itemIndex;
	Item* item = new Item[itemIndex.GetArrIndex() + 1];
	
	Item it;
	it.AddItem(item[0], 1, "단검", 1, 'A');
	it.AddItem(item[1], 2, "단검", 2, 'A');
	it.AddItem(item[2], 3, "갑옷", 1, 'B');
	it.AddItem(item[3], 4, "반지", 2, 'B');
	it.AddItem(item[4], 5, "반지", 3, 'S');

	it.Show(item, 2, 3);
	it.Show(item, 1, 2);
	it.Show(item, 3, 4);
}


//Item class 를 만들고 
//	index, name, level, grade 등 데이타 맴버를 추가하세요.
//	Show, SetGrade, GetGrade, GradeUp 함수등을 구성하세요.

//Item을 추가하고 for문으로 출력하세요.

//EX)
//	1, 단검, 1, 'A'
//	2, 단검, 2, 'A'
//	3, 갑옷, 1, 'B'
//	4, 반지, 2, 'B'
//	5, 반지, 3, 'S'

//------------------------------------------------------------------------------------------------
//Item class 를 이용하여
//임의이 아이템을 5개 이상 생성하여 배열로 선언하고 (아이템 리스트)
//같은 등급의 아이템 2개를 합성하여 상위등급의 아이템 하나로 만드는 시스템을 만드세요.
//등급 : S A B C
//합성후 네임은 재료1을 따르며, 레벨은 1로 초기화되고, 재료가 되는 아이템은 삭제 됩니다.

//EX)
//합성 전 아이템 목록 출력
//	1, 단검, 1, 'A'
//	2, 단검, 2, 'A'
//	3, 갑옷, 1, 'B'
//	4, 반지, 2, 'B'
//	5, 반지, 3, 'S'
//
//합성 재료를 입력하세요.
//합성 재료1 : 1
//합성 재료2 : 2
//합성 결과  : 6, 단검, 1, 'S'
//
//합성 후 아이템 목록 출력
//	3. 갑옷, 1, 'B'
//	4. 반지, 2, 'B'
//	5, 반지, 3, 'S'
//	6, 단검, 1, 'S'
