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

    MyWinSocket serverSocket;

    UINT nSocketPort = 7777;

    if (!serverSocket.Create(SOCK_STREAM))
        return 0;

    if (!serverSocket.Bind(nSocketPort))
        return 0;

    if (!serverSocket.Listen(1))
        return 0;

    if (!serverSocket.CreateEventHandle())
        return 0;

    while (true)
    {
        if (serverSocket.WaitForMultipleEvents() == WSA_WAIT_FAILED)
            continue;

        serverSocket.EnumNetworkEvents();

        UINT eventState = serverSocket.GetEventState();

        int* eventErrorCode = serverSocket.GetErrorCode();
        
        if (eventState == FD_ACCEPT)
        {
            int errorCode = eventErrorCode[FD_ACCEPT_BIT];

            serverSocket.OnAccept(errorCode);
        }

        else if (eventState == FD_READ || eventState == FD_WRITE)
        {
            int errorCode = eventErrorCode[FD_READ_BIT] | eventErrorCode[FD_WRITE_BIT];
            
            if (!(serverSocket.OnSend(errorCode)))
                continue;
        }
    }

    // 해제도 한번만.
    WSACleanup();
    return 0;
}
