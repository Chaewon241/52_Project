// 맨 처음에 해야하는거

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#include "../MyFirstTcpServer/WinSock.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

// 자주 쓰는거 래핑

int main()
{
    // 윈속 초기화는 프로그램 실행시에 한번만 해야한다.
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    MyWinSocket ws;

    UINT nSocketPort = 7777;

    ws.Create(SOCK_STREAM);
    ws.Bind(nSocketPort);
    ws.Listen(1);
    ws.CreateEventHandle();

    while (true)
    {
        if (ws.WaitForMultipleEvents() == WSA_WAIT_FAILED)
            continue;

        ws.EnumNetworkEvents();

        UINT eventState = ws.GetEventState();

        int* eventErrorCode = ws.GetErrorCode();
        
        if (eventState == FD_ACCEPT)
        {
            int errorCode = eventErrorCode[FD_ACCEPT_BIT];

            ws.OnAccept(errorCode);
        }

        else if (eventState == FD_READ || eventState == FD_WRITE)
        {
            int errorCode = eventErrorCode[FD_READ_BIT] | eventErrorCode[FD_WRITE_BIT];
            
            if (!(ws.OnSend(errorCode)))
                continue;
        }
    }

    // 해제도 한번만.
    WSACleanup();
    return 0;
}
