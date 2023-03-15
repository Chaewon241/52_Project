#include <iostream>
#include <random>
#include <time.h>

using namespace std;

bool ck_SameNum(int n1, int n2)
{
    if (n1 == n2)
        return true;
    else
        return false;
}

bool ck_RandNum(int n1, int n2, int n3)
{
    if (n1 != n2 && n2 != n3 && n1 != n3)
        return true;
    else
        return false;
}

int main()
{
    srand((unsigned)time(NULL));
    int arr[3] = { 0 };

    for (;;)
    {
        int a = rand() % 10;
        int b = rand() % 10;
        int c = rand() % 10;

        if (ck_RandNum(a, b, c))
        {
            arr[0] = a;
            arr[1] = b;
            arr[2] = c;
            break;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        int out = 0;
        int ball = 0;
        int strike = 0;

        int b_arr[3] = { 0 };
        cin >> b_arr[0] >> b_arr[1] >> b_arr[2];

        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (ck_SameNum(arr[j], b_arr[k]) && j == k)
                {
                    strike++;
                    break;
                }

                else if (ck_SameNum(arr[j], b_arr[k]))
                {
                    ball++;
                    break;
                }

                else
                {
                    if (k == 2)
                        out++;
                    else
                        continue;
                }
            }
        }

        if (strike == 3)
        {
            cout << "승리하셨습니다." << endl;
            break;
        }

        if (i == 8 && strike != 3)
        {
            cout << "실패하셨습니다." << endl;
            cout << "정답은 " << arr[0] << " " << arr[1] << " " << arr[2] << endl;
            break;
        }
        cout << "strike: " << strike << " ball: " << ball << " out: " << out << endl;
    }
}