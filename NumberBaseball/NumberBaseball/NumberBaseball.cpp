#include <iostream>
#include <random>
#include <time.h>

using namespace std;

bool ck_SBO(int* c_arr, int* p_arr)
{
    int out = 0;
    int ball = 0;
    int strike = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (c_arr[i] == p_arr[j] && i == j) 
            {
                strike++;
                break;
            }
            else if (c_arr[i] == p_arr[j])
            {
                ball++;
                break;
            }
            else
            {
                if (j == 2)
                    out++;
                else
                    continue;
            }
                
        }
    }
    cout << "strike: " << strike << " ball: " << ball << " out: " << out << endl;

    if (strike == 3)
        return true;
    else
        return false;
}

// 랜덤한 3가지 숫자를 저장하기
// 세 수가 다르면 true, 한 경우라도 같은 숫자가 있으면 false 리턴해주기
bool ck_Num(int n1, int n2, int n3)
{
    if (n1 != n2 && n2 != n3 && n1 != n3)
        return true;
    else
        return false;
}

int main()
{
    // seed 설정
    srand((unsigned)time(NULL));

    // 세 숫자를 담을 배열을 0으로 초기화
    int arr[3] = { 0 };
    int* arr_add = &arr[3];

    // 서로 다른 랜덤한 세 숫자를 찾기 위한 무한루프
    for (;;)
    {
        // 변수 3개에 10까지 범위의 랜덤한 숫자 선언
        int a = rand() % 10;
        int b = rand() % 10;
        int c = rand() % 10;

        // 세 숫자가 다른지 확인하는 함수
        if (ck_Num(a, b, c) == true)    // 다 다를 때
        {
            // 배열에 각각 값을 넣어주기
            arr[0] = a;
            arr[1] = b;
            arr[2] = c;
            break;
        }
    }

    // 9번 실행할거임
    for (int i = 0; i < 9; i++)
    {

        // 사용자가 입력한 세 숫자를 담을 배열 초기화
        int b_arr[3] = { 0 };

        while (true)
        {
            cout << "숫자 3개를 입력하세요: ";
            cin >> b_arr[0] >> b_arr[1] >> b_arr[2];

            // 예외처리
            // 숫자만 입력
            if (!cin)
            {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "다시 입력하세요." << endl;
                continue;
            }

            // 세 숫자가 다 다르고 10보다 작은 수일 때 빠져나오기
            if (ck_Num(b_arr[0], b_arr[1], b_arr[2]) && b_arr[0] < 10 && b_arr[1] < 10 && b_arr[2] < 10)
                break;
            else
                cout << "다시 입력하세요." << endl;
        }
        bool result = ck_SBO(arr, b_arr);
        if (result)
        {
            cout << "승리했습니다." << endl;
            break;
        }

        if (!result && i == 8)
        {
            cout << "실패하셨습니다." << endl;
            // 정답 알려주기
            cout << "정답은 " << arr[0] << " " << arr[1] << " " << arr[2] << endl;
        }
    }
}

