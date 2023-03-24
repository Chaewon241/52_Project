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
void infoDraw();

int main()
{
    init();

    while (1)
    {
        titleDraw();
        int menuCode = menuDraw();
        if (menuCode == 0)
        {

        }
        else if (menuCode == 1)
        {
            infoDraw();
        }
        else if (menuCode == 2)
        {
            return 0;
        }
        system("cls");
    }    
}

void infoDraw()
{
    system("cls");
    printf("\n\n");
    printf("조작법\n\n");

    while (1)
    {
        if (keyControl() == SUBMIT)
        {
            break;
        }
    }
}

int keyControl()
{

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
    system("mode con cols=50 lines=30 | title 게임명");

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 0;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void titleDraw()
{

}

void gotoxy(int x, int y)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(consoleHandle, pos);
}