#include <iostream>

using namespace std;

/*
* 1. 아이템 등급 이넘 클래스로 만들기.
* 2. 아이템 클래스를 관리하는 아이템 매니저 클래스로 분리.
* 3. 클래스는 다른 파일로 생성.
*
*/

enum class EItemGrade : int
{
    C = 0, // C등급
    B = 1, // B등급
    A = 2, // A등급
    S = 3, // S등급
    Max    // 열거값의 최대값
};

// 풀 개념.
// 인덱스 풀을 이용해서, 삭제된 아이템의 인덱스는 제거 하고, 새로 생성되는 인덱스는 비어있는 인덱스를 반환하게.
// 1 2 3 4 5
// 아이템 5개를 넣으면 1~5 모두 사용.
// 아이템 합성시, 1과 2가 사라지므로 1,2를 다시 반환. ( 1, 2 사용할 수 있는 상태 )
// 새로운 아이템이 나올때 다시 1을 배정.
// 아이템 인덱스 ( 고유번호로 ) arr에 접근가능?
class ItemManager
{};

// 아이템 배열 타입 정의
using ItmeArr = Item*;

// 아이템 포인터 타입 정의
using ItemPtr = Item*;

class Item
{
private:
    // 초기화는 생성자를 이용
    // 아이템 관리 매니저가 들고있는게 깔끔해보임
    int    m_maxIndex = 5; // 매니저에 들고있고, 어차피 아이템 생성및 관리를 매니저가 할태니깐? 최초에 0으로 들고있다가 생성될떄마다 +1 하면될듯?
    int    m_arrIndex = 4;

    // 모든 멤버 변수를 초기화
    //  아이템의 멤버변수
    int    m_index = 1; // static 으로 가져가기?
    int    m_level;
    string m_name;
    char   m_grade;

public:
    void   SetIndex(int index) { m_index = index; }
    int    GetIndex() { return m_index; }
    int    GetArrIndex() { return m_arrIndex; }

    void   SetName(string name) { m_name = name; }
    string GetName() { return m_name; }

    void   SetLevel(int level) { m_level = level; }
    int    GetLevel() { return m_level; }

    void   SetGrade(char grade) { m_grade = grade; }
    char   GetGrade() { return m_grade; }

    // 매니저
    void AddItem(Item& item, int index, string name, int level, char grade)
    {
        item.SetIndex(index);
        item.SetName(name);
        item.SetLevel(level);
        item.SetGrade(grade);
    }

    // 매니저
    void PrintList(ItmeArr item)
    {
        for (int i = 0; i < m_arrIndex + 1; i++)
        {
            cout << item[i].GetIndex() << ", " << item[i].GetName() << ", " << item[i].GetLevel() <<
                ", " << item[i].GetGrade() << endl;
        }
        cout << endl;
    }

    // 매니저
    // 매개변수 이름 의미있게 만들기
    void Show(ItmeArr item, int n1, int n2)
    {
        cout << "변경 전" << endl;
        PrintList(item);
        cout << "변경 후" << endl;
        MergeItem(item, n1, n2);
    }

    EItemGrade GradeUp(EItemGrade grade)
    {
        switch (grade)
        {
        case EItemGrade::C:
            break;
        case EItemGrade::B:
            break;
        case EItemGrade::A:
            break;
        case EItemGrade::S:
            break;
        case EItemGrade::Max:
        default:
            break;
        }
    }

    void MergeItem(ItmeArr item, int item1, int item2)
    {
        // 포인터를 다루는 모든 부분은 널체크 필요
        if (!item)
            return;

        int itemIdx1 = -1;
        int itemIdx2 = -1;

        // 지금은 아이템 클래스가 m_arrIndex를 가지고 있어서 알 수 있는데,
        // 배열을 주소값으로만 함수 전달 할때는 배열이 끝나는 지점도 같이 명시해주면서 넘겨야
        // 배열을 초과하는 주소값을 접근 하지 않음

        for (int i = 0; i < m_arrIndex + 1; i++)
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

        if (item[itemidx1].GetGrade() != item[itemidx2].GetGrade())
        {
            cout << "강화 앙 실패띠~" << endl;
            cout << endl;
            return;
        }

        string itemname = item[itemidx1].GetName();
        char grade = GradeUp(item[itemidx1].GetGrade());
        int tmp = 0;

        for (int i = 0; i < m_arrIndex + 1; i++)
        {
            if (i == itemidx1 || i == itemidx2)
            {
                continue;
            }

            AddItem(item[tmp++], item[i].GetIndex(), item[i].GetName(), item[i].GetLevel(), item[i].GetGrade());
        }

        AddItem(item[--m_arrIndex], ++m_maxIndex, itemName, 1, grade);

        PrintList(item);
    }
};

int main(void)
{
    Item itemIndex; // ?
    ItmeArr item = new Item[itemIndex.GetArrIndex() + 1];

    // 매니저
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
//   index, name, level, grade 등 데이타 맴버를 추가하세요.
//   Show, SetGrade, GetGrade, GradeUp 함수등을 구성하세요.

//Item을 추가하고 for문으로 출력하세요.

//EX)
//   1, 단검, 1, 'A'
//   2, 단검, 2, 'A'
//   3, 갑옷, 1, 'B'
//   4, 반지, 2, 'B'
//   5, 반지, 3, 'S'

//------------------------------------------------------------------------------------------------
//Item class 를 이용하여
//임의이 아이템을 5개 이상 생성하여 배열로 선언하고 (아이템 리스트)
//같은 등급의 아이템 2개를 합성하여 상위등급의 아이템 하나로 만드는 시스템을 만드세요.
//등급 : S A B C
//합성후 네임은 재료1을 따르며, 레벨은 1로 초기화되고, 재료가 되는 아이템은 삭제 됩니다.

//EX)
//합성 전 아이템 목록 출력
//   1, 단검, 1, 'A'
//   2, 단검, 2, 'A'
//   3, 갑옷, 1, 'B'
//   4, 반지, 2, 'B'
//   5, 반지, 3, 'S'
//
//합성 재료를 입력하세요.
//합성 재료1 : 1
//합성 재료2 : 2
//합성 결과  : 6, 단검, 1, 'S'
//
//합성 후 아이템 목록 출력
//   3. 갑옷, 1, 'B'
//   4. 반지, 2, 'B'
//   5, 반지, 3, 'S'
//   6, 단검, 1, 'S'