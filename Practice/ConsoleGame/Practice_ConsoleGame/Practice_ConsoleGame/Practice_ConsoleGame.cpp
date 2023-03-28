#include "main.h"
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
        if (menuCode == 0)
        {

        }
        else if (menuCode == 1)
        {
            infoDraw();
        }
        else if (menuCode == 2)
        {
            return 0;
        }
        system("cls");
    }    
}