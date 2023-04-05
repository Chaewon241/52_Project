#pragma once
#include "RenderSystem.h"
#include "InputSystem.h"

enum {
	black,
	blue,
	green,
	cyan,
	red,
	purple,
	brown,
	lightgray,
	darkgray,
	lightblue,
	lightgreen,
	lightcyan,
	lightred,
	lightpurple,
	yellow,
	white
};

void MainMenu();
void titleDraw();
void infoDraw();
int menuDraw();
void setColor(int forground, int background);