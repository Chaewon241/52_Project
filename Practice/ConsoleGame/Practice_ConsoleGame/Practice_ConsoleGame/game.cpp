#include "game.h"
#include <cassert>



ULONGLONG previousTime;
ULONGLONG currentTime;
ULONGLONG deltaTime;
//��墸��
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

// UI -> ������, �ð� ���
//void drawUI(int* x, int* y, int* item_light)
//{
//    setColor(white, black);
//    gotoxy(*x, *y); // �� �����ʹ� �ʿ� ��������?
//    printf("���� : %02d ��", item_light);
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
    if (GetAsyncKeyState('A') & 0x0001) //���� 'A'
    {
        Set(USER_CMD_LEFT, true);
    }

    if (GetAsyncKeyState('D') & 0x0001) //������ 'D'
    {
        Set(USER_CMD_RIGHT, true);
    }

    if (GetAsyncKeyState('W') & 0x0001) //���� 'W'
    {
        Set(USER_CMD_UP, true);
    }
    if (GetAsyncKeyState('S') & 0x0001) //�Ʒ��� 'S'
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
    // Ű �Է°� ȭ�� ��°� ���� ���� ������ �и��ؼ� ������ ���ô�.
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
    printf("�� Ż���غ�����~");
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

            // �����۵� ����
            // ����
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

int menuDraw()
{
    int x = 80;
    int y = 30;

    GotoXY(x - 2, y);
    printf("�� ���ӽ���");
    GotoXY(x, y + 2);
    printf(" ��������");
    GotoXY(x, y + 4);
    printf(" ��������");

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
                printf("��");
            }
            break;

        case DOWN:
            if (y < 34)
            {
                GotoXY(x - 2, y);
                printf("  ");
                y += 2;
                GotoXY(x - 2, y);
                printf("��");
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

