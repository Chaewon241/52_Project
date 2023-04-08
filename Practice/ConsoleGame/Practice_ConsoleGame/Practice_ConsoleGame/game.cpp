#include "game.h"
#include <cassert>
#include <queue>
#include <functional>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>

ULONGLONG previousTime;
ULONGLONG currentTime;
ULONGLONG deltaTime;
ULONGLONG pauseTime;

//▲▼◀▶
int tmpMap[ROWS][COLS];
int maze[ROWS][COLS];
int playing;

char map1[41][122] = {};
char map2[41][122] = {};
char map3[41][122] = {};


int x_dir[4] = { -1, 1, 0, 0 };
int y_dir[4] = { 0, 0, -1, 1 };

COORD curPlayerPos;

int updateCount;
int fixedUpdateCount;
int fixedUpdateCount1;
int itemTimer;
int item = 0;

const int MAX_KEY = 6;
bool inputKeyTable[MAX_KEY];

const int showPath = 5000;
bool isPath = false;

// UI -> 아이템, 시간 등등
//void drawUI(int* x, int* y, int* item_light)
//{
//    setColor(white, black);
//    gotoxy(*x, *y); // 이 포인터는 필요 없을지도?
//    printf("조명 : %02d 개", item_light);
//}



std::vector<std::pair<int, int>> bfs(int start_y, int start_x)
{
	struct bfsStruct
	{
		std::pair<int, int> xy;
		std::vector<std::pair<int, int>> path;
	};

	std::queue<bfsStruct> q;
	bool visited[41][122] = { 0 };

	visited[start_x][start_y] = true;

	{
		bfsStruct temp;
		temp.xy = std::make_pair(start_x, start_y);

		q.push(temp);
	}

	while (!q.empty())
	{
		int x = q.front().xy.first;
		int y = q.front().xy.second;

		for (int i = 0; i < 4; ++i)
		{
			int x_new = x + x_dir[i];
			int y_new = y + y_dir[i];

			if ((0 <= x_new && x_new < 41) && (0 <= y_new && y_new < 122)
				&& !visited[x_new][y_new])
			{
				if (map1[x_new][y_new] == 'g')
				{
					return q.front().path;
				}
				if (map1[x_new][y_new] == '#')
				{
					continue;
				}

				visited[x_new][y_new] = true;


				bfsStruct temp;
				temp.xy = std::make_pair(x_new, y_new);
				temp.path = q.front().path;
				temp.path.push_back(temp.xy);

				q.push(temp);

				//q.push(std::make_pair(x_new, y_new));
			}
		}

		q.pop();
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

bool IsESCCmdOn()
{
	return inputKeyTable[USER_CMD_ESCAPE];
}

bool IsItemCmdOn()
{
	return inputKeyTable[USER_CMD_ITEM];
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
		Set(USER_CMD_ESCAPE, true);
	}
	if (GetAsyncKeyState('Q') & 0x0001) //아이템 사용 'Q'
	{
		Set(USER_CMD_ITEM, true);
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
		item += 1;
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
	ErasePath();
}

void UpdatePlayer()
{
	static ULONGLONG elapsedTime;

	elapsedTime += GetDeltaTime();
	pauseTime = 0;

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

	if (IsItemCmdOn())
	{
		Set(USER_CMD_ITEM, false);
		

		if (item > 0)
		{
			item--;
			isPath = true;
			std::vector<std::pair<int, int>> pathXY;
			pathXY = bfs(curPlayerPos.X, curPlayerPos.Y);

			for (int i = 0; i < pathXY.size(); i++)
			{
				GotoXY(pathXY[i].second, pathXY[i].first);
				printf("o");
			}

			
		}
	}

	if (IsESCCmdOn())
	{
		Set(USER_CMD_ESCAPE, false);
		// 다시 시작하는 화면 출력하는 함수, 시간 멈춰야함
		// 거기서 예 아니오 누르기

		/*GotoXY(170, 7);
		setColor(white, black);
		printf("다시 시작하시겠습니까?");
		GotoXY(170, 8);
		setColor(white, black);
		printf("예(y) / 아니오(n)");
		while (1)
		{
			int k = keyControl();
			if (k == YES)
			{

			}
			else if (k == NO)
			{
				GotoXY(170, 7);
				setColor(black, black);
				printf("다시 시작하시겠습니까?");
				GotoXY(170, 8);
				setColor(black, black);
				printf("예(y) / 아니오(n)");
				break;
			}
		}*/
	}
}

void ErasePath()
{
	static ULONGLONG elapsedTime = 0;

	if (isPath) 
	{
		elapsedTime += GetDeltaTime();
		std::vector<std::pair<int, int>> pathXY;
		pathXY = bfs(curPlayerPos.X, curPlayerPos.Y);

		while (elapsedTime >= showPath)
		{
			for (int i = 0; i < pathXY.size(); i++)
			{
				GotoXY(pathXY[i].second, pathXY[i].first);
				printf(" ");
			}

			elapsedTime -= showPath;
			isPath = false;
		}
	}
}

void UpdatedelapsedTime()
{
	static ULONGLONG elapsedTime = 0;
	int playTime;
	elapsedTime += GetDeltaTime();
	while (elapsedTime >= 10)
	{
		playTime = 120 - (fixedUpdateCount / 100);
		if (playTime == 0)
		{
			playing = 0;
			break;
		}
		fixedUpdateCount += 1;
		GotoXY(170, 5);

		printf("%03d", playTime);
		elapsedTime = 0;
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
	fixedUpdateCount1 = 0;
	itemTimer = 0;

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

	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 122; j++)
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
			// 경로 알려주기
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
	GotoXY(50, 10);
	printf("           ##\n");
	GotoXY(50, 11);
	printf("##  ##    ###     #####     ####              #####    #####   ##  ##    ####\n");
	GotoXY(50, 12);
	printf("######     ##     ##  ##   ##  ##            ##  ##       ##   ######   ######\n");
	GotoXY(50, 13);
	printf("##  ##     ##     ##       ##  ##             #####   ######   ##  ##   ##\n");
	GotoXY(50, 14);
	printf("##  ##   ######   ##        ####                 ##    #####   ##  ##    ####\n");
	GotoXY(50, 15);
	printf("                                              #####                              ");
}

// 맵 텍스트 파일 마니마니 생성하고 랜덤으로 하기
void generate_maze() {
	srand((unsigned int)time(NULL));

	const int max = 1024;
	char line[max];
	char* pLine;
	int randnum = rand() % 2;
	FILE* fp1;

	if (randnum == 0)
		fp1 = fopen("Miro1.txt", "r");
	else if (randnum == 1)
		fp1 = fopen("Miro2.txt", "r");

	if (fp1 == NULL)
		exit(1);

	int i = 0;
	while (!feof(fp1)) {
		pLine = fgets(line, 1024, fp1);
		for (int j = 0; j < 122; j++)
		{
			map1[i][j] = pLine[j];
		}
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
	else if (tmp == 'y' || tmp == 'Y')
		return YES;
	else if (tmp == 'n' || tmp == 'N')
		return NO;
}

