#include <iostream>

#define MAX 10
#define MAX_STACK_SIZE 50

enum EMAZETYPE { PATH, WALL, VISIT, BACK };

using namespace std;

// 아쉬운 점: 전역변수 사용, 깔끔하게 Back 처리

int g_Maze[MAX][MAX] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct MazePosition
{
    int x, y;
    MazePosition(int initX = -1, int initY = -1)
    {
        x = initX;
        y = initY;
    }
    MazePosition* pNextNode = nullptr;

};

MazePosition g_Start(1, 1);
MazePosition g_Goal(8, 8);
MazePosition g_Current;

struct MazePositionStack
{
    MazePosition* pTopNode = nullptr;
    
    bool IsEmpty()
    {
        return pTopNode == nullptr;
    }

    bool Push(MazePosition data)
    {
        if (pTopNode == nullptr) {
            pTopNode = new MazePosition(data);
            return true;
        }
        // 기존의 topNode를 잠시 백업
        MazePosition* tempData = pTopNode;

        pTopNode = new MazePosition(data);
        pTopNode->pNextNode = tempData;
        return true;
    }

    bool Pop() // 메모리 주소값 전달
    {
        if (IsEmpty())
            return false;
        MazePosition* tempData = pTopNode->pNextNode;
        delete pTopNode;
        pTopNode = tempData;

        g_Maze[g_Current.x][g_Current.y] = EMAZETYPE::BACK;
        MazePosition tmp(pTopNode->x, pTopNode->y);
        g_Current = tmp;
        g_Maze[g_Current.x][g_Current.y] = EMAZETYPE::BACK;
        return true;
    }

    ~MazePositionStack()
    {
        cout << "Destruct" << endl;
        while (Pop() == true)
        {
            Pop();
        }
    }
};

MazePosition position;
MazePositionStack Stack;

bool CheckMazeType(MazePosition position, int Type)
{
    return g_Maze[position.x][position.y] == Type;
}

bool FindNextPosition(MazePosition input)
{
    int dy[4] = { 1,-1,0,0 };
    int dx[4] = { 0,0,1,-1 };

    int nx = 0, ny = 0;

    for (int i = 0; i < 4; i++)
    {
        nx = input.x + dx[i];
        ny = input.y + dy[i];

        if (nx < 1 || ny < 1 || nx > 8 || ny > 8)
            continue;
        else if (g_Maze[nx][ny] == EMAZETYPE::VISIT || g_Maze[nx][ny] == EMAZETYPE::BACK)
            continue;
        else if (CheckMazeType(MazePosition(nx, ny), 0))
        {
            MazePosition position = MazePosition(nx, ny);

            g_Current = position;
            return 1;
        }
    }
    return 0;
}

void MoveInMaze(MazePosition NewPosition)
{
    g_Maze[NewPosition.x][NewPosition.y] = EMAZETYPE::VISIT;

    Stack.Push(NewPosition);
}

void BackInMaze()
{
    Stack.Pop();

}

void ShowMaze()
{
    for (int x = 0; x < MAX; x++)
    {
        for (int y = 0; y < MAX; y++)
        {
            if (g_Maze[x][y] == EMAZETYPE::PATH)
                cout << ' ';
            else if (g_Maze[x][y] == EMAZETYPE::WALL)
                cout << '#';
            else if (g_Maze[x][y] == EMAZETYPE::VISIT)
                cout << 'V';
            else if (g_Maze[x][y] == EMAZETYPE::BACK)
                cout << 'B';
        }
        cout << "\n";
    }
}

int main()
{
    // 첫 이동
    g_Current = g_Start;
    g_Maze[g_Current.x][g_Current.y] = EMAZETYPE::VISIT;
    Stack.Push(g_Current);

    while ((g_Goal.x != g_Current.x) || (g_Goal.y != g_Current.y))
    {
        bool result = FindNextPosition(g_Current);

        if (result)
        {
            MoveInMaze(g_Current);
        }
        else
        {
            BackInMaze();
        }
    }

    ShowMaze();
    Stack.~MazePositionStack();
}
