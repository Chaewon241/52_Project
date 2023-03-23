#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

using namespace std;

void init();
void titleDraw();
void gotoxy(int, int);
int keyControl();
int menuDraw();

int main()
{
    init();
    titleDraw();
    menuDraw();
}

int keyControl()
{
    if (_kbhit()) {
        char tmp = _getch();

        if (tmp == 'w' || tmp == 'W')
            return UP;
        else if (tmp == 'a' || tmp == 'A')
            return LEFT;
        else if (tmp == 's' || tmp == 'S')
            return DOWN;
        else if (tmp == 'd' || tmp == 'D')
            return RIGHT;
        else if (tmp == ' ') // 스페이스바 선택
            return SUBMIT;
    }
}

int menuDraw()
{
    int x = 24;
    int y = 12;

    gotoxy(x - 2, y);
    cout << "> 게임시작";
    gotoxy(x, y + 1);
    printf("게임정보");
    gotoxy(x, y + 2);
    printf("게임종료");

    while (1)
    {
        int n = keyControl();
        switch (n)
        {
            case UP: 
                if (y > 12)
                {
                    gotoxy(x - 2, y);
                    printf(" ");
                    gotoxy(x - 2, --y);
                    printf(">");
                }
                break;
            
            case DOWN: 
                if (y < 14)
                {
                    gotoxy(x - 2, y);
                    printf(" ");
                    gotoxy(x - 2, ++y);
                    printf(">");
                }
                break;
            
            case SUBMIT:
                return y - 12;
        }
    }
}

void init()
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); // 콘솔창 크기 정보를 구조체에 저장
    MoveWindow(console, r.left, r.top, 800, 600, TRUE); // 콘솔창 크기 조절
}

void titleDraw()
{
    cout << "Hello World!\n" << endl;
    cout << "Hello World!\n" << endl;
    cout << "Hello World!\n" << endl;
    cout << "Hello World!\n" << endl;
    cout << "Hello World!\n" << endl;
    cout << "Hello World!\n" << endl;

}

void gotoxy(int x, int y)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(consoleHandle, pos);
}