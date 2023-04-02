#pragma once
#include "main.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

void gLoop(int);
void init();
void titleDraw();
void gotoxy(int, int);
int keyControl();
int menuDraw();
void infoDraw();
void Greeting();
void drawMap();
