#include "game.h"
#include <cassert>



ULONGLONG previousTime;
ULONGLONG currentTime;
ULONGLONG deltaTime;
//▲▼◀▶
int tmpMap[ROWS][COLS];
int maze[ROWS][COLS];
int playing = 1;
std::string map1[61] = {};
std::string map2[61] = {};
std::string map3[61] = {};

COORD curPlayerPos;

int updateCount;
int item_light = 0;

const int MAX_KEY = 4;
bool inputKeyTable[MAX_KEY];

// UI -> 아이템, 시간 등등
//void drawUI(int* x, int* y, int* item_light)
//{
//    setColor(white, black);
//    gotoxy(*x, *y); // 이 포인터는 필요 없을지도?
//    printf("조명 : %02d 개", item_light);
//}
void InitTime()
{
    currentTime = previousTime = GetTickCount64();
}

void UpdateTime()
{
    previousTime = currentTime;
    currentTime = GetTickCount64();

    deltaTime = currentTime - previousTime;
}

ULONGLONG GetDeltaTime()
{
    return deltaTime;
}

void Set(const int keyIdx, bool bOn)
{
    assert(keyIdx >= 0 && keyIdx < MAX_KEY);

    inputKeyTable[keyIdx] = bOn;
}

bool IsLeftCmdOn()
{
    return inputKeyTable[USER_CMD_LEFT];
}

bool IsRightCmdOn()
{
    return inputKeyTable[USER_CMD_RIGHT];
}

bool IsUpCmdOn()
{
    return inputKeyTable[USER_CMD_UP];
}

bool IsDownCmdOn()
{
    return inputKeyTable[USER_CMD_DOWN];
}

void ProcessInput()
{
    UpdateInput();
}

void UpdateInput()
{
    if (GetAsyncKeyState('A') & 0x0001) //왼쪽 'A'
    {
        Set(USER_CMD_LEFT, true);
    }

    if (GetAsyncKeyState('D') & 0x0001) //오른쪽 'D'
    {
        Set(USER_CMD_RIGHT, true);
    }

    if (GetAsyncKeyState('W') & 0x0001) //위쪽 'W'
    {
        Set(USER_CMD_UP, true);
    }
    if (GetAsyncKeyState('S') & 0x0001) //아래쪽 'S'
    {
        Set(USER_CMD_DOWN, true);
    }
}

void move(int x, int y)
{
    char mapObject = map1[y][x];
    setColor(white, black);

    if (mapObject == ' ')
    {
        GotoXY(curPlayerPos.X, curPlayerPos.Y);
        printf(" ");
        setColor(cyan, black);
        GotoXY(x, y);
        printf("@");

        curPlayerPos.X = x;
        curPlayerPos.Y = y;
    }
    else if (mapObject == 'l')
    {
        item_light += 1;
        map1[y][x] = ' ';
        GotoXY(x, y);
        printf(" ");
    }
    else if (mapObject == 'g')
    {
        playing = 0;
    }
}

void Update()
{
    updateCount += 1;

    UpdatePlayer();

}

void UpdatePlayer()
{
    // 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
    static ULONGLONG elapsedTime;

    elapsedTime += GetDeltaTime();

    while (elapsedTime >= playerMoveSpeed)
    {
        UpdatePlayerPosition();

        elapsedTime -= playerMoveSpeed;
    }
}

void UpdatePlayerPosition()
{
    char mapObject = map1[curPlayerPos.X][curPlayerPos.Y];
    if (IsLeftCmdOn())
    {
        Set(USER_CMD_LEFT, false);
        move(curPlayerPos.X - 1, curPlayerPos.Y);
    }

    if (IsRightCmdOn())
    {
        Set(USER_CMD_RIGHT, false);
        move(curPlayerPos.X + 1, curPlayerPos.Y);
    }

    if (IsUpCmdOn())
    {
        Set(USER_CMD_UP, false);
        move(curPlayerPos.X, curPlayerPos.Y - 1);
    }

    if (IsDownCmdOn())
    {
        Set(USER_CMD_DOWN, false);
        move(curPlayerPos.X, curPlayerPos.Y + 1);
    }
}

void gLoop()
{
    int x, y;
    
    generate_maze();
    drawMap(&x, &y);
    while (playing)
    {
        
        //drawUI(&x, &y);
        UpdateTime();
        
        ProcessInput();
        Update();
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

    for (int i = 0; i < 61; i++)
    {
        for (int j = 0; j < 152; j++)
        {
            if (map1[i][j] == ' ')
            {
                setColor(black, black);
                printf(" ");
            }
                
            else if (map1[i][j] == '#')
            {
                setColor(white, white);
                printf("#");
            }
                
            else if (map1[i][j] == 'p')
            {
                curPlayerPos.X = j;
                curPlayerPos.Y = i;
                setColor(cyan, black);
                printf("@");
            }

            else if (map1[i][j] == 'g')
            {
                setColor(lightgreen, black);
                printf("O");
            }

            // 아이템들 구현
            // 조명
            else if (map1[i][j] == 'l')
            {
                setColor(yellow, black);
                printf("*");
            }
                
        }
        printf("\n");
    }
    setColor(white, black);
    // Sleep(3000);
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
    else if (tmp == '\r')
        return SUBMIT;
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

int menuDraw()
{
    int x = 80;
    int y = 30;

    GotoXY(x - 2, y);
    printf("▶ 게임시작");
    GotoXY(x, y + 2);
    printf(" 게임정보");
    GotoXY(x, y + 4);
    printf(" 게임종료");

    while (1)
    {
        int n = keyControl();
        switch (n)
        {
        case UP:
            if (y > 30)
            {
                GotoXY(x - 2, y);
                printf("  ");
                y -= 2;
                GotoXY(x - 2, y);
                printf("▶");
            }
            break;

        case DOWN:
            if (y < 34)
            {
                GotoXY(x - 2, y);
                printf("  ");
                y += 2;
                GotoXY(x - 2, y);
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
    printf("           ##\n");
    printf("##  ##    ###     #####     ####              #####    #####   ##  ##    ####\n");
    printf("######     ##     ##  ##   ##  ##            ##  ##       ##   ######   ######\n");
    printf("##  ##     ##     ##       ##  ##             #####   ######   ##  ##   ##\n");
    printf("##  ##   ######   ##        ####                 ##    #####   ##  ##    ####\n");
    printf("                                              #####                              ");
}

void generate_maze() {
    const int max = 1024;
    char line[max];
    char* pLine;
    FILE* fp1;
    fp1 = fopen("Miro1.txt", "r");
    if (fp1 == NULL)
        exit(1);

    int i = 0;
    while(!feof(fp1)) {
        pLine = fgets(line, 1024, fp1);
        map1[i] = pLine;
        i++;
    }
    fclose(fp1);
}

