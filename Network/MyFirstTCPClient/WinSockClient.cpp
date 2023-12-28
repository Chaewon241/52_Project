#include "WinSockClient.h"
#include "WinSock.h"
#include "NetWorkSystem.h"

#include <iostream>
#include <cstring>

using namespace std;

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

#pragma warnings(disable: 4996)

bool WinSockClient::Initialize()
{
    m_ClientWinSock = new MyWinSocket;

    if (!m_ClientWinSock->Create(SOCK_STREAM))
        return false;

    // m_sendBuffer 와 m_recvBuffer 를 동적 할당, 나중에 해제 필수
    m_sendBuffer = new char[SND_BUF_SIZE];
    m_recvBuffer = new char[RCV_BUF_SIZE];

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

void WinSockClient::Send()
{
    int sendLen = ::send(m_ClientWinSock->GetSocket(), m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
    if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
        cout << "send Error " << ::WSAGetLastError() << endl;
    }
    else
    {
        cout << "send " << sendLen << " " << endl;
    }
}

bool WinSockClient::DoUpdate()
{
    // isEventOccurs의 필요성이 없어보임.
    

    return true;
}

void WinSockClient::PressKey()
{
    const char* c = "Left";

    memcpy(m_sendBuffer, c, sizeof(c));

    Send();
}

void WinSockClient::CloseSocket()
{
   closesocket(m_ClientWinSock->GetSocket());
}

int WinSockClient::isEventOccurs()
{
    return ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);
}

void WinSockClient::OnConnect(int nErrorCode)
{
    m_isConnected = true;
    printf("Connected\n");
}

void WinSockClient::OnReceive(int nErrorCode)
{
    int recvBytes = ::recv(m_ClientWinSock->GetSocket(), m_recvBuffer, RCV_BUF_SIZE, 0);
    if (recvBytes == SOCKET_ERROR)
    {
        cout << "Read Error" << endl;
        return;
    }

    // 읽을거 없을 때
    if (recvBytes == 0)
    {
        cout << "Disconnected" << endl;
        return;
    }
    cout << "Recv: " << m_recvBuffer << endl;
}
