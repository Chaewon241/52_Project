﻿#include "main.h"
#include "game.h"
#include "util.h"
using namespace std;

int main()
{
    init();

    while (1)
    {
        titleDraw();
        int menuCode = menuDraw();
        if (menuCode == 30)
        {
            //Greeting();
            gLoop(0);
            gLoop(1);
        }
        else if (menuCode == 32)
        {
            infoDraw();
        }
        else if (menuCode == 34)
        {
            return 0;
        }
        system("cls");
    }    
}