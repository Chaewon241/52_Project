#include "game.h"
#include <cassert>
#include <queue>
#include <functional>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

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
int item = 0;

const int MAX_KEY = 6;
bool inputKeyTable[MAX_KEY];

const int showPath = 5000; 
bool isPath = false;
bool isPass = false;

std::vector<std::pair<int, int>> pathXY;

 // UI -> 아이템, 시간 등등
void drawUI()
{
    setColor(yellow, black);
    GotoXY(125, 8);
    printf("아이템(Q) : %d 개", item);
	GotoXY(125, 12);
	printf("종료를 원한다면 ESC를 누르세요");
}

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
		setColor(white, black);
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
		int x = 60, y = 15;
		system("cls");
		setColor(green, black);
		GotoXY(x, y++);
		printf("#######  ##        ######      ##   ######");
		GotoXY(x, y++);
		printf("##   ##  ##       #######    #####  #######");
		GotoXY(x, y++);
		printf("##       ##       ##         ## ##  ##   ##");
		GotoXY(x, y++);
		printf("###      ##       #######   ##  ##  ######");
		GotoXY(x, y++);
		printf("###      ##       ##        ######  ##   ##");
		GotoXY(x, y++);
		printf("###  ##  #######  #######  ##   ##  ##   ##");
		GotoXY(x, y++);
		printf("#######   ######   ######  ##   ##  ##   ##");
		GotoXY(x, y++);
		Sleep(3000);

		playing = 0;
	}
}

void Update()
{
	updateCount += 1;

	UpdatePlayer();
	ErasePath();
	drawUI();
	
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
			pathXY = bfs(curPlayerPos.X, curPlayerPos.Y);

			for (int i = 0; i < pathXY.size(); i++)
			{
				setColor(lightblue, black);
				GotoXY(pathXY[i].second, pathXY[i].first);
				printf("o");
			}
		}
	}

	if (IsESCCmdOn())
	{
		Set(USER_CMD_ESCAPE, false);
		playing = 0;
	}
}

void ErasePath()
{
	static ULONGLONG elapsedTime = 0;

	if (isPath) 
	{
		elapsedTime += GetDeltaTime();
		
		while (elapsedTime >= showPath)
		{
			for (int i = 0; i < pathXY.size(); i++)
			{
				GotoXY(pathXY[i].second, pathXY[i].first);
				if (map1[pathXY[i].first][pathXY[i].second] == 'l') 
				{
					setColor(yellow, black);
					printf("*");
				}

				else 
				{
					printf(" ");
				}	
			}
			setColor(white, black);
			GotoXY(curPlayerPos.X, curPlayerPos.Y);
			printf("@");
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
		playTime = 60 - (fixedUpdateCount / 100);
		if (playTime == 0)
		{
			setColor(red, black);
			int x = 60, y = 15;
			system("cls");
			GotoXY(x, y++);
			printf("######      ##    ######  ##");
			GotoXY(x, y++);
			printf("##        #####   ######  ##");
			GotoXY(x, y++);
			printf("##        ## ##     ##    ##");
			GotoXY(x, y++);
			printf("#####    ##  ##     ##    ##");
			GotoXY(x, y++);
			printf("###      ######     ##    ##");
			GotoXY(x, y++);
			printf("###     ##   ##   ######  #######");
			GotoXY(x, y++);
			printf("###     ##   ##   ######  #######");
			GotoXY(x, y++);
			Sleep(3000);

			playing = 0;
			break;
		}
		fixedUpdateCount += 1;
		GotoXY(130, 5);
		setColor(red, black);
		printf("%02d초", playTime);
		elapsedTime = 0;
	}
}

void startGame()
{
	int x = 0, y = 0;
	generate_maze();
	drawMap();
}

void gLoop()
{
	InitTime();
	startGame();

	playing = 1;
	fixedUpdateCount = 0;
	item = 0;
	curPlayerPos.X = 0;
	curPlayerPos.Y = 0;

	GotoXY(curPlayerPos.X, curPlayerPos.Y);
	setColor(white, black);
	printf("@");

	while (playing)
	{
		UpdateTime();
		ProcessInput();
		UpdatedelapsedTime();

		Update();
	}
}

void drawMap()
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
				setColor(green, green);
				printf("#");
			}

			else if (map1[i][j] == 'g')
			{
				setColor(red, black);
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

int menuDraw()
{
	int x = 70;
	int y = 30;

	GotoXY(x - 2, y);
	printf("▶ 게임시작");
	GotoXY(x, y + 2);
	printf(" 게임방법");
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

void infoDraw()
{
	system("cls");
	int x = 70, y = 15;
	GotoXY(x, y++);
	setColor(green, black);
	printf("★★게임 방법★★");
	GotoXY(x - 10, y+2);
	setColor(white, black);
	printf("1. 10가지의 랜덤한 미로가 생성됩니다.");
	GotoXY(x - 10, y + 4);
	printf("2. w a s d로 조작할 수 있습니다.");
	GotoXY(x - 10, y + 6);
	printf("3. 120초 안에 미로를 탈출하세요.");
	GotoXY(x - 10, y + 8);
	printf("4. 아이템을 먹으면 5초동안 길을 보여줍니다.");

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
	int x = 40, y = 15;
	setColor(blue, black);
	GotoXY(x, y++);
	printf("           ##");
	GotoXY(x, y++);
	printf("##  ##    ###     #####     ####              #####    #####   ##  ##    ####");
	GotoXY(x, y++);
	printf("######     ##     ##  ##   ##  ##            ##  ##     ####   ######   ######");
	GotoXY(x, y++);
	printf("##  ##     ##     ##       ##  ##             #####    #   #   ##  ##   ##");
	GotoXY(x, y++);
	printf("##  ##   ######   ##        ####                 ##    #####   ##  ##    ####");
	GotoXY(x, y++);
	printf("                                              #####                              ");
	setColor(white, black);
	GotoXY(x + 15, y + 5);
	printf("w a로 메뉴를 선택 하고 엔터를 눌러 실행하세요.");
}

// 맵 텍스트 파일 마니마니 생성하고 랜덤으로 하기
void generate_maze() {
	srand((unsigned int)time(NULL));

	const int max = 1024;
	char line[max];
	char* pLine;
	int randnum = rand() % 10;
	FILE* fp1 = NULL;

	std::string str = "Miro";
	str += std::to_string(randnum);
	std::string filename = str + ".txt";
	
	fopen_s(&fp1, filename.c_str(), "r");

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

