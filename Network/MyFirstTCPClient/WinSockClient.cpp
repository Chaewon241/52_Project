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

//void WinSockClient::NetUpdate()
//{
//    if (m_readBytes)
//    {
//        memcpy(m_sendBuffer, m_recvBuffer, m_readBytes);
//
//        m_readBytes = 0;
//    }
//
//    if (m_writeBytes)
//    {
//        int nSent = Send();
//
//        if (nSent > 0)
//        {
//            m_writeBytes -= nSent;
//
//            if (m_writeBytes > 0)
//            {
//                memmove(m_sendBuffer, m_sendBuffer + nSent, m_writeBytes);
//            }
//        }
//        // 소켓 버퍼가 가득 차서 전송이 불가능
//        else if (nSent == 0)
//        {
//
//        }
//        // 소켓 에러가 발생함. 우짤까
//        else
//        {
//
//        }
//    }
//}

void WinSockClient::CloseSocket()
{
   closesocket(m_ClientWinSock->GetSocket());
}

int WinSockClient::EventState()
{
    return ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);
}