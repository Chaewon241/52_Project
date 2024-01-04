#pragma once
#include "main.h"
#include "MiroMap.h"
#include "game.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4	// 엔터키로 했음
#define YES 5
#define NO 6

#define ROWS 30 // 미로의 행 수
#define COLS 100 // 미로의 열 수


const int USER_CMD_LEFT = 0;
const int USER_CMD_RIGHT = 1;
const int USER_CMD_UP = 2;
const int USER_CMD_DOWN = 3;
const int USER_CMD_ESCAPE = 4;
const int USER_CMD_ITEM = 5;

const int playerMoveSpeed = 200;

//void dfs();
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
void UpdatedelapsedTime();

void generate_maze();

void gLoop();
void move(int, int);
void init();
void titleDraw();
int menuDraw();
void infoDraw();
void drawMap();
void ErasePath();
