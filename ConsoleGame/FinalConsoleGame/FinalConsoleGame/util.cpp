#include "util.h"

void MainMenu()
{
    system("mode con cols=200 lines=50 | title 게임명");

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 0;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

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

void titleDraw()
{

}

int menuDraw()
{
    int x = 80;
    int y = 30;

    render::GotoXY(x - 2, y);
    printf("▶ 게임시작");
    render::GotoXY(x, y + 2);
    printf(" 게임정보");
    render::GotoXY(x, y + 4);
    printf(" 게임종료");

    while (1)
    {
        int n = keyControl();
        switch (n)
        {
        case UP:
            if (y > 30)
            {
                render::GotoXY(x - 2, y);
                printf("  ");
                y -= 2;
                render::GotoXY(x - 2, y);
                printf("▶");
            }
            break;

        case DOWN:
            if (y < 34)
            {
                render::GotoXY(x - 2, y);
                printf("  ");
                y += 2;
                render::GotoXY(x - 2, y);
                printf("▶");
            }
            break;

        case SUBMIT:
            return y;
        }
    }
}


void setColor(int forground, int background)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int code = forground + background * 16;
    SetConsoleTextAttribute(consoleHandle, code);
}