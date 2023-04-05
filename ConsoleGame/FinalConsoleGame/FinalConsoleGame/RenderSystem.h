#pragma once
#include <windows.h>
#include <stdio.h>
#include "Game.h"
#include "util.h"

namespace render
{
    void InitScreen();
    void ScreenRelease();

    void ScreenFlipping();
    void ScreenClear();
    void GotoXY(int x, int y);

    void ScreenDraw(int x, int y, char c);
    void ScreenDraw2(int x, int y, const char* pStr);

    SMALL_RECT GetPlayerMovableRect();
    void DrawBorder();
};
void Render();
void drawMap(int* x, int* y);