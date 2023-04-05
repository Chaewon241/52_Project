#include "Game.h"

void StartGame()
{
    render::InitScreen();

    global::playerMovableRect = render::GetPlayerMovableRect();

    render::DrawBorder(); // 벽을 그려 놓자!

    // 플레이어 시작 위치 설정
    global::curPlayerPos.X = 2;
    global::curPlayerPos.Y = 3;
    
    DrawPlayer();
}

void DrawPlayer()
{
    render::ScreenDraw(global::curPlayerPos.X, global::curPlayerPos.Y, '@');
}

bool IsGameRun()
{
    return true;
}

void Update(int key)
{
    global::time::updateCount += 1;

    UpdatePlayer(key);
}

void UpdatePlayer(int key)
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= global::playerMoveSpeed)
    {
        UpdatePlayerPosition(key);

        elapsedTime -= global::playerMoveSpeed;
    }
}

void UpdatePlayerPosition(int key)
{
    if (key == 0)
    {
        global::curPlayerPos.Y--;
    }
    if (key == 1)
    {
        global::curPlayerPos.Y++;
    }
    if (key == 2)
    {
        global::curPlayerPos.X--;
    }
    if (key == 3)
    {
        global::curPlayerPos.X++;
    }
}