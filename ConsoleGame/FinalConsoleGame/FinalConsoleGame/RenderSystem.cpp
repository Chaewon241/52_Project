#include "RenderSystem.h"


namespace render
{
    
    bool bScreenIndex;
    HANDLE hScreen[2];

    SMALL_RECT consoleScreenSize;
    INT consoleScreenX;
    INT consoleScreenY;

    SMALL_RECT updateScreenSize;
    INT updateScreenX;
    INT updateScreenY;
    

    SMALL_RECT GetPlayerMovableRect()
    {
        return updateScreenSize;
    }

    HANDLE GetScreenHandle()
    {
        int index = (bScreenIndex ? 1 : 0);

        return hScreen[index];
    }

    void GotoXY(int x, int y)
    {
        COORD Cur = { x, y };

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
    }

    void InitScreen()
    {
        hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        CONSOLE_CURSOR_INFO cursorInfo = { 0, };
        cursorInfo.bVisible = 0;
        cursorInfo.dwSize = 1;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

        consoleScreenSize.Left = csbi.srWindow.Left;
        consoleScreenSize.Right = csbi.srWindow.Right;
        consoleScreenSize.Bottom = csbi.srWindow.Bottom;
        consoleScreenSize.Top = csbi.srWindow.Top;

        consoleScreenX = csbi.dwSize.X;
        consoleScreenY = csbi.dwSize.Y;

        updateScreenSize.Left = consoleScreenSize.Left + 2;
        updateScreenSize.Right = consoleScreenSize.Right - 10;
        updateScreenSize.Bottom = consoleScreenSize.Bottom - 2;
        updateScreenSize.Top = consoleScreenSize.Top + 2;

        updateScreenX = consoleScreenX - 12;
        updateScreenY = consoleScreenY - 4;
    }

    void ScreenFlipping()
    {
        SetConsoleActiveScreenBuffer(GetScreenHandle());
        bScreenIndex = !bScreenIndex;
    }

    void ScreenClear()
    {
        COORD Coor = { updateScreenSize.Left, updateScreenSize.Top };
        DWORD dw;

        for (int y = 0; y < updateScreenY; y++)
        {
            Coor.Y = updateScreenSize.Top + y;

            FillConsoleOutputCharacter(GetScreenHandle(), ' ', updateScreenX, Coor, &dw);
        }
    }

    void ScreenRelease()
    {
        CloseHandle(hScreen[0]);
        CloseHandle(hScreen[1]);
    }

    // 문구들 더블버퍼링 -> 몇 스테이지인지랑 아이템 표시
    void ScreenDraw(int x, int y, char c)
    {
        DWORD dw;
        COORD Cur = { x, y };
        
        char buffer[10];
        sprintf_s(buffer, "%c", c);

        SetConsoleCursorPosition(GetScreenHandle(), Cur);

        WriteFile(GetScreenHandle(), buffer, 1, &dw, NULL);
    }

    // 미로 더블버퍼링
    void ScreenDraw2(int x, int y, const char* pStr)
    {
        DWORD dw;
        COORD Cur = { x, y };

        SetConsoleCursorPosition(hScreen[bScreenIndex], Cur);

        WriteFile(hScreen[bScreenIndex], pStr, strlen(pStr), &dw, NULL);
    }

    void draw_map(char buffermap[30][100])
    {
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 100; j++)
            {

            }
        }
    }

    void DrawBorder()
    {
        for (int x = updateScreenSize.Left - 1; x < updateScreenSize.Right + 1; x++)
        {
            ScreenDraw2(x, updateScreenSize.Top - 1, "#");
        }

        for (int x = updateScreenSize.Left - 1; x < updateScreenSize.Right + 1; x++)
        {
            ScreenDraw2(x, updateScreenSize.Bottom + 1, "#");
        }

        for (int y = updateScreenSize.Top - 1; y < updateScreenSize.Bottom + 1; y++)
        {
            ScreenDraw2(updateScreenSize.Left - 1, y, "#");
        }

        for (int y = updateScreenSize.Top - 1; y < updateScreenSize.Bottom + 1; y++)
        {
            ScreenDraw2(updateScreenSize.Right + 1, y, "#");
        }
    }

};

void Render()
{
    //ScreenClear();
    //switch (global::mapIndex)
    //{
    //case 1:
    //    ScreenDraw2(updateScreenX, updateScreenY, global::map1);
    //    break;
    //case 2:
    //    ScreenDraw2(updateScreenX, updateScreenY, global::map2);
    //    break;
    //case 3:
    //    ScreenDraw2(updateScreenX, updateScreenY, global::map3);
    //    break;
    ///*case 4:
    //    ScreenDraw2(updateScreenX, updateScreenY, global::map3);
    //    break;
    //case 5:
    //    ScreenDraw2(updateScreenX, updateScreenY, global::map3);
    //    break;*/
    //default:
    //    break;
    //}

    //global::mapIndex++;
    //ScreenFlipping();
}
void drawMap(int* x, int* y)
{
    system("cls");
    int h, w;
    for (h = 0; h < 30; h++)
    {
        for (w = 0; w < 100; w++)
        {
            char temp = global::tmpmap[h][w];
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