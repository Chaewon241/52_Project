#include "InputSystem.h"

int keyControl()
{
    char tmp = _getch();

    if (tmp == 'w' || tmp == 'W')
        return UP;
    else if (tmp == 'a' || tmp == 'A')
        return LEFT;
    else if (tmp == 's' || tmp == 'S')
        return DOWN;
    else if (tmp == 'd' || tmp == 'D')
        return RIGHT;
    else if (tmp == '\r') // 스페이스바는 ' ', 이거는 엔터 선택
        return SUBMIT;
}
