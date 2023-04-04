#include "game.h"
#include "util.h"

//▲▼◀▶

char tempMap[30][100];

char map1[30][100] = {
    {"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"},
    {"1p00000l0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000g1"},
    {"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"}
};

char map2[30][100] = {
    {"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"},
    {"1p0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"},
    {"1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000g1"},
    {"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"}
};

// UI -> 아이템, 시간 등등
//void drawUI(int* x, int* y, int* item_light)
//{
//    setColor(white, black);
//    gotoxy(*x, *y); // 이 포인터는 필요 없을지도?
//    printf("조명 : %02d 개", item_light);
//}

void move(int* x, int* y, int _x, int _y, int* item_light)
{
    char mapObject = tempMap[*y + _y][*x + _x];
    setColor(white, black);

    if (mapObject == '0')
    {
        gotoxy(*x, *y);
        printf(" ");
        setColor(cyan, black);
        gotoxy(*x + _x, *y + _y);
        printf("@");

        *x += _x;
        *y += _y; 
    }
    else if (mapObject == 'l')
    {
        *item_light += 1;
        tempMap[*y + _y][*x + _x] = '0';
        gotoxy(*x + _x, *y + _y);
        printf(" ");
    }

}

void gLoop(int mapCode)
{
    int x, y;
    int item_light = 0;
    int playing = 1;

    if (mapCode == 0)
    {
        memcpy(tempMap, map1, sizeof(tempMap));
    }
    else if (mapCode == 1)
    {
        memcpy(tempMap, map2, sizeof(tempMap));
    }
    drawMap(&x, &y);

    while (playing)
    {
        // drawUI(&x, &y);
        switch (keyControl())
        {
        case UP:
            move(&x, &y, 0, -1, &item_light);
            break;
        case DOWN:
            move(&x, &y, 0, 1, &item_light);
            break;
        case RIGHT:
            move(&x, &y, 1, 0, &item_light);
            break;
        case LEFT:
            move(&x, &y, -1, 0, &item_light);
            break;
        case SUBMIT:
            playing = 0;
        }
    }
}

void Greeting()
{
    system("cls");
    printf("잘 탈출해보세요~");
    Sleep(2000);
}

void drawMap(int* x, int* y)
{
    system("cls");
    int h, w;
    for (h = 0; h < 30; h++)
    {
        for (w = 0; w < 100; w++)
        {
            char temp = tempMap[h][w];
            if (temp == '0')
            {
                setColor(black, black);
                printf(" ");
            }
                
            else if (temp == '1')
            {
                setColor(white, white);
                printf("#");
            }
                
            else if (temp == 'p')
            {
                *x = w;
                *y = h;
                setColor(cyan, black);
                printf("@");
            }
            else if (temp == 'g') {
                setColor(lightgreen, black);
                printf("O");
            }
            // 아이템들 구현
            // 조명
            else if (temp == 'l') {
                setColor(lightgreen, black);
                printf("*");
            }
                
        }
        printf("\n");
    }
    setColor(white, black);
    // Sleep(3000);
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
    int x = 80;
    int y = 30;

    gotoxy(x - 2, y);
    printf("▶ 게임시작");
    gotoxy(x, y + 2);
    printf(" 게임정보");
    gotoxy(x, y + 4);
    printf(" 게임종료");

    while (1)
    {
        int n = keyControl();
        switch (n)
        {
        case UP:
            if (y > 30)
            {
                gotoxy(x - 2, y);
                printf("  ");
                y -= 2;
                gotoxy(x - 2, y);
                printf("▶");
            }
            break;

        case DOWN:
            if (y < 34)
            {
                gotoxy(x - 2, y);
                printf("  ");
                y += 2;
                gotoxy(x - 2, y);
                printf("▶");
            }
            break;

        case SUBMIT:
            return y;
        }
    }
}

void titleDraw()
{

}
