#include <iostream>
#include <random>
#include <time.h>

using namespace std;

// 랜덤한 3가지 숫자를 저장하기
// 세 수가 다르면 true, 한 경우라도 같은 숫자가 있으면 false 리턴해주기
bool ck_Num(int n1, int n2, int n3)
{
    if (n1 != n2 && n2 != n3 && n1 != n3)
        return true;
    else
        return false;
}

bool Victory(int s)
{
    if (s == 3)
        return true;
    else
        return false;
}

int main()
{
    int out;
    int ball;
    int strike;

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
        // 한 상황에서 out ball strike를 선언해준다
        out = 0;
        ball = 0;
        strike = 0;

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
            }

            // 세 숫자가 다 다르고 10보다 작은 수일 때 빠져나오기
            if (ck_Num(b_arr[0], b_arr[1], b_arr[2]) && b_arr[0] < 10 && b_arr[1] < 10 && b_arr[2] < 10)
                break;
            else
                cout << "다시 입력하세요." << endl;
        }
        

        // 랜덤한 세 숫자와 사용자가 입력한 세 숫자 비교하기
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                // arr[j](랜덤한 세 숫자 중 하나)와 b_arr[k](사용자가 입력한 세 숫자중 하나)
                // 둘이 같은지 비교하는 함수가 true이고 인덱스(자리)가 같을 때
                if (arr[j] == b_arr[k] && j == k)
                {
                    // strike를 하나 올려줌
                    strike++;
                    break;
                }

                //arr[j](랜덤한 세 숫자 중 하나)와 b_arr[k](사용자가 입력한 세 숫자중 하나)
                // 둘이 숫자는 똑같은데 자리가 다를 때
                else if (arr[j] == b_arr[k])
                {
                    // ball을 하나 올려줌
                    ball++;
                    break;
                }

                else
                {
                    // 마지막까지 ball과 strike를 판단하지 못했을 때
                    if (k == 2)
                        // out을 올려줌
                        out++;
                    // ball을 찾기 위해 continue
                    else
                        continue;
                }
            }
        }

        cout << "strike: " << strike << " ball: " << ball << " out: " << out << endl;

        // for문을 통해 strike ball out이 몇개인지 나오는데
        // strike가 3이면 승리하고 break
        if (Victory(strike))
        {
            cout << "승리하셨습니다." << endl;
            break;
        }
    }

    if (!Victory(strike))
    {
        cout << "실패하셨습니다." << endl;
        // 정답 알려주기
        cout << "정답은 " << arr[0] << " " << arr[1] << " " << arr[2] << endl;
    }
}