#pragma once
#include "main.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4	// ����Ű�� ����

void gLoop(int);
void move(int*, int*, int, int, int*);
void init();
void titleDraw();
void gotoxy(int, int);
int keyControl();
int menuDraw();
void infoDraw();
void Greeting();
void drawMap(int*, int*);
//void drawUI(int*, int*, int*); �����۰����� UI
