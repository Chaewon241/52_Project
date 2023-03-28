#include "game.h"

//▲▼◀▶

void infoDraw()
{
    system("cls");
    printf("\n\n");
    printf("조작법\n\n");
    printf("으아아아아\n\n");
    printf("으아아아아아아\n\n");

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
    else if (tmp == '\r') // 스페이스바는 ' ', 이거는 엔터 선택
        return SUBMIT;

}

int menuDraw()
{
    int x = 24;
    int y = 12;

    gotoxy(x - 2, y);
    printf("▶ 게임시작");
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
                printf("▶");
            }
            break;

        case DOWN:
            if (y < 14)
            {
                gotoxy(x - 2, y);
                printf(" ");
                gotoxy(x - 2, ++y);
                printf("▶");
            }
            break;

        case SUBMIT:
            return y - 12;
        }
    }
}

void titleDraw()
{

}
