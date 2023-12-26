#include "WinSockClient.h"
#include "WinSock.h"
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

    const char* c = "Hello";
    memcpy(m_sendBuffer, c, sizeof(c));

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
    //m_ClientWinSock->DisConnect();
}

bool WinSockClient::DoUpdate()
{
    // 이벤트 감지
    // 이벤트객체가 하나라도 실행 가능한 상태가 되었는지 확인.
    
    int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

    if (index == WSA_WAIT_FAILED)
        return false;

    if (index == WSA_WAIT_TIMEOUT) return true;

    // 이건 매뉴얼 입니당 원래 항상 WSA_WAIT_EVENT_0 더해줘서 하나 빼줘야함.
    index -= WSA_WAIT_EVENT_0;

    // 소켓의 내부 정보
    // WSAEnumNetworkEvents 함수는 소켓에 발생한 네트워크 이벤트를 확인하는 데 사용
    // 주로 WSAEvent와 함께 사용되어 소켓에서 발생한 이벤트가 있는지 확인하고 
    // 이벤트의 상태를 얻는 데 활용
    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(m_ClientWinSock->GetSocket(), wsaEvents[index], &networkEvents) == SOCKET_ERROR)
        return 0;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
            return false;
    }

    OnConnect(networkEvents.iErrorCode[FD_CONNECT_BIT]);

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return false;
        }
        char buffer[RCV_BUF_SIZE] = {};
        int recvBytes = ::recv(m_ClientWinSock->GetSocket(), buffer, RCV_BUF_SIZE, 0);
        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return false;
        }

        // 읽을거 없을 때
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return false;
        }
        cout << "Recv: " << buffer << endl;
    }

    // 여기까지 순서: 이벤트 객체 상태 확인(WSAWaitForMultipleEvents)
    // -> 소켓에서 발생한 네트워크 이벤트에 대한 정보 얻기(WSAEnumNetworkEvents)


    //// receive 뭔가 읽어올게 있다면
    //if (networkEvents.lNetworkEvents & FD_READ)
    //{
    //    if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
    //    {
    //        cout << "Read Error" << endl;
    //        return false;
    //    }
    //    char buffer[BUFSIZE] = {};
    //    int recvBytes = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
    //    if (recvBytes == SOCKET_ERROR)
    //    {
    //        cout << "Read Error" << endl;
    //        return false;
    //    }

    //    // 읽을거 없을 때
    //    if (recvBytes == 0)
    //    {
    //        cout << "Disconnected" << endl;
    //        return false;
    //    }
    //    cout << "Recv: " << buffer << endl;
    //}

    //if (networkEvents.lNetworkEvents & FD_CLOSE)
    //{
    //    if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
    //    {
    //        cout << "Close Error" << endl;
    //    }
    //    m_isConnected = false;
    //}

    //if (m_isConnected)
    //{
    //    Sleep(1000);

    //    int sendLen = ::send(m_clientSocket, m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
    //    if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
    //        cout << "send Error " << ::WSAGetLastError() << endl;
    //    }
    //    else
    //    {
    //        cout << "send " << sendLen << " " << endl;
    //    }
    //}
    return true;
}

void WinSockClient::PressKey()
{
    const char* c = "Left";

    memcpy(m_sendBuffer, c, sizeof(c));
}

void WinSockClient::CloseSocket()
{
   // closesocket(m_clientSocket);
}

void WinSockClient::OnConnect(int nErrorCode)
{
    m_isConnected = true;

    m_ClientWinSock->OnConnect(nErrorCode);
    cout << "Connected" << endl;

    // send 해주자.
    int sendLen = ::send(m_ClientWinSock->GetSocket(), m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
    if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
        cout << "send Error " << ::WSAGetLastError() << endl;
    }
    else
    {
        cout << "send " << sendLen << " " << endl;
    }
}