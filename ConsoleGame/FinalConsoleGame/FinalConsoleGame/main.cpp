#include "main.h"

int main()
{
    MainMenu();

    while (1)
    {
        titleDraw();
        int menuCode = menuDraw();
        if (menuCode == 30)
        {
            global::time::InitTime();
            StartGame();
            while (IsGameRun())
            {
                int x, y;
                if (global::mapIndex == 0)
                    memcpy(global::tmpmap, global::map1, sizeof(global::tmpmap));
                else if (global::mapIndex == 1)
                    memcpy(global::tmpmap, global::map2, sizeof(global::tmpmap));
                drawMap(&x, &y);

                global::time::UpdateTime();
                int push_key = keyControl();
                Update(push_key);
                Render();
                global::mapIndex++;
            }
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
