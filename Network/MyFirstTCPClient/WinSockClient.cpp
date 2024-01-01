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

int WinSockClient::Send()
{
    int sendLen = ::send(m_ClientWinSock->GetSocket(), m_sendBuffer, m_writeBytes, 0);
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

void WinSockClient::NetUpdate()
{
    if (m_readBytes)
    {
        memcpy(m_sendBuffer, m_recvBuffer, m_readBytes);

        m_readBytes = 0;
    }

    if (m_writeBytes)
    {
        int nSent = Send();

        if (nSent > 0)
        {
            m_writeBytes -= nSent;

            if (m_writeBytes > 0)
            {
                memmove(m_sendBuffer, m_sendBuffer + nSent, m_writeBytes);
            }
        }
        // 소켓 버퍼가 가득 차서 전송이 불가능
        else if (nSent == 0)
        {

        }
        // 소켓 에러가 발생함. 우짤까
        else
        {

        }
    }
}

void WinSockClient::SendAllChatting(char* inputStr)
{
    m_sendBuffer = inputStr;
    m_writeBytes += strlen(inputStr) + 1;
}

// 네트워크 시스템에서 받은 버퍼들 복사해주기
void WinSockClient::RecvReadBuffer(char* readBuffer)
{
    m_recvBuffer = readBuffer;
}

void WinSockClient::WriteSendBuffer(char* sendBuffer, int size)
{
    m_sendBuffer = sendBuffer;
    m_writeBytes += size;
}

void WinSockClient::CloseSocket()
{
   closesocket(m_ClientWinSock->GetSocket());
}

int WinSockClient::EventState()
{
    return ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);
}