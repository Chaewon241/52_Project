#include "util.h"

void MainMenu()
{
    system("mode con cols=200 lines=50 | title ���Ӹ�");

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
    printf("���۹�\n\n");
    printf("���ƾƾƾ�\n\n");
    printf("���ƾƾƾƾƾ�\n\n");

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
    printf("�� ���ӽ���");
    render::GotoXY(x, y + 2);
    printf(" ��������");
    render::GotoXY(x, y + 4);
    printf(" ��������");

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
                printf("��");
            }
            break;

        case DOWN:
            if (y < 34)
            {
                render::GotoXY(x - 2, y);
                printf("  ");
                y += 2;
                render::GotoXY(x - 2, y);
                printf("��");
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