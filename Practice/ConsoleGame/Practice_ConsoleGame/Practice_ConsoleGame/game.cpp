#include "game.h"

//��墸��

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
    else if (tmp == '\r') // �����̽��ٴ� ' ', �̰Ŵ� ���� ����
        return SUBMIT;

}

int menuDraw()
{
    int x = 24;
    int y = 12;

    gotoxy(x - 2, y);
    printf("�� ���ӽ���");
    gotoxy(x, y + 1);
    printf("��������");
    gotoxy(x, y + 2);
    printf("��������");

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
                printf("��");
            }
            break;

        case DOWN:
            if (y < 14)
            {
                gotoxy(x - 2, y);
                printf(" ");
                gotoxy(x - 2, ++y);
                printf("��");
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
