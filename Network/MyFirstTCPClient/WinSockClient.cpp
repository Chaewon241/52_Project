#include "NetWorkSystem.h"
#include "WinSockClient.h"

#include <cstring>
#include <iostream>

using namespace std;

#pragma warnings(disable: 4996)

bool WinSockClient::Initialize()
{
    m_ClientWinSock = new MyWinSocket;

    if (!m_ClientWinSock->Create(SOCK_STREAM))
        return false;

    return true;
}

// 클라이언트 소켓,
bool WinSockClient::Connect()
{
    if (!m_ClientWinSock->CreateEventHandle())
        return false;

    m_ClientWinSock->Connect(7777, "127.0.0.1");
    wsaEvents.push_back(m_ClientWinSock->GetEventHandle());

    return true;
}

void WinSockClient::DisConnect()
{
    m_ClientWinSock->DisConnect();
}

int WinSockClient::Send(char* sendBuffer, int size)
{
    int sendLen = ::send(m_ClientWinSock->GetSocket(), sendBuffer, size, 0);
    if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
        cout << "send Error " << ::WSAGetLastError() << endl;
    }
    else
    {
        cout << "send " << sendLen << " " << endl;
    }
    return sendLen;
}

bool WinSockClient::Update()
{
    // inputSystem의 update 넣어주기

    return true;
}

void WinSockClient::CloseSocket()
{
   closesocket(m_ClientWinSock->GetSocket());
}

int WinSockClient::EventState()
{
    return ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);
}