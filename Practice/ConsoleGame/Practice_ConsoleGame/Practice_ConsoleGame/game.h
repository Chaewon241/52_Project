#pragma once
#include "main.h"
#include "MiroMap.h"
#include "game.h"
#include "util.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4	// ����Ű�� ����

#define ROWS 30 // �̷��� �� ��
#define COLS 100 // �̷��� �� ��


const int USER_CMD_LEFT = 0;
const int USER_CMD_RIGHT = 1;
const int USER_CMD_UP = 2;
const int USER_CMD_DOWN = 3;
const int ENTER_KEY_INDEX = 4;

const int playerMoveSpeed = 200;

void Set(const int, bool);
bool IsLeftCmdOn();
bool IsRightCmdOn();
bool IsUpCmdOn();
bool IsDownCmdOn();

void UpdatePlayer();
void UpdatePlayerPosition();
void UpdateInput();
void ProcessInput();
int keyControl();

void generate_maze();

void gLoop();
void move(int, int);
void init();
void titleDraw();
int menuDraw();
void infoDraw();
void Greeting();
void drawMap(int*, int*);

//void drawUI(int*, int*, int*); �����۰����� UI
