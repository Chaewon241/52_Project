#include "game.h"
#include <cassert>
#include <queue>
#include <functional>
#include <utility>

ULONGLONG previousTime;
ULONGLONG currentTime;
ULONGLONG deltaTime;
//▲▼◀▶
int tmpMap[ROWS][COLS];
int maze[ROWS][COLS];
int playing;
std::string map1[61] = {};
std::string map2[61] = {};
std::string map3[61] = {};
int bfsMap[61][152] = { 0 };
bool visited[61][152] = { 0 };

int x_dir[4] = { -1, 1, 0, 0 }; 
int y_dir[4] = { 0, 0, -1, 1 };

COORD curPlayerPos;

int updateCount;
int fixedUpdateCount;
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

std::queue<std::pair<int, int>> q;

void bfs(int start_x, int start_y) {

    visited[start_x][start_y] = 1;          // 입력 받은 시작 좌표를 방문
    q.push(std::make_pair(start_x, start_y));     // queue 에 삽입
    bfsMap[start_x][start_y]++;               // 시작 좌표까지 이동한 칸을 1로 지정

    // 모든 좌표를 탐색할 때까지 반복
    while (!q.empty()) {

        // queue 의 front 좌표를, 현재 좌표로 지정
        int x = q.front().first;
        int y = q.front().second;

        // qeueu 의 front 좌표 제거
        q.pop();

        // 현재 좌표와 상하좌우로 인접한 좌표 확인
        for (int i = 0; i < 4; ++i) {

            // 현재 좌표와 상하좌우로 인접한 좌표
            int x_new = x + x_dir[i];
            int y_new = y + y_dir[i];

            // 인접한 좌표가, 미로 내에 존재하는지, 방문한 적이 없는지, 이동이 가능한지 확인
            if ((0 <= x_new && x_new < 61) && (0 <= y_new && y_new < 152)
                && !visited[x_new][y_new] && maze[x_new][y_new] == 1) {

                visited[x_new][y_new] = 1;              // 인접 좌표는 방문한 것으로 저장
                q.push(std::make_pair(x_new, y_new));        // 인접 좌표를 queue 에 삽입
                bfsMap[x_new][y_new] = bfsMap[x][y] + 1;    // 인접 좌표까지 이동한 거리 저장
            }
        }
    }
}

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
    if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) //'ESC'
    {
        Set(VK_ESCAPE, true);
    }
    if (GetAsyncKeyState('Y') & 0x0001) //예 'Y'
    {
        Set(USER_CMD_YES, true);
    }
    if (GetAsyncKeyState('N') & 0x0001) //ㄴ 'N'
    {
        Set(USER_CMD_NO, true);
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
    //bfs(curPlayerPos.X, curPlayerPos.Y);

}

void UpdatePlayer()
{
    static ULONGLONG elapsedTime;

    elapsedTime += GetDeltaTime();

    while (elapsedTime >= playerMoveSpeed)
    {
        UpdatePlayerPosition();

        elapsedTime -= playerMoveSpeed;
    }
}

void UpdatedelapsedTime()
{
    static ULONGLONG elapsedTime = 0;
    
    elapsedTime += GetDeltaTime();
    int playTime;
    while (elapsedTime >= 10)
    {
        playTime = 5 - (fixedUpdateCount / 100);
        if (playTime == 0)
        {
            playing = 0;
            break;
        }
        fixedUpdateCount += 1;
        GotoXY(170, 5);
        
        printf("%02d", playTime);
        elapsedTime -= 10;
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

//void Render()
//{
//    ScreenClear();
//}

void gLoop()
{
    int x, y;
    InitTime();
    generate_maze();
    drawMap(&x, &y);
    playing = 1;
    fixedUpdateCount = 0;
    while (playing)
    {
        //drawUI(&x, &y);
        UpdateTime();
        ProcessInput();
        UpdatedelapsedTime();

        Update();
        //bfs(curPlayerPos.X, curPlayerPos.Y);
        //Render();
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
    printf("게임정보");
    GotoXY(x, y + 4);
    printf("게임종료");

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

