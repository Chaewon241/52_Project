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

    // m_sendBuffer �� m_recvBuffer �� ���� �Ҵ�, ���߿� ���� �ʼ�
    m_sendBuffer = new char[SND_BUF_SIZE];
    m_recvBuffer = new char[RCV_BUF_SIZE];

    const char* c = "Hello";
    memcpy(m_sendBuffer, c, sizeof(c));

    return true;
}

// Ŭ���̾�Ʈ ����,
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
    // �̺�Ʈ ����
    // �̺�Ʈ��ü�� �ϳ��� ���� ������ ���°� �Ǿ����� Ȯ��.
    
    int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

    if (index == WSA_WAIT_FAILED)
        return false;

    if (index == WSA_WAIT_TIMEOUT) return true;

    // �̰� �Ŵ��� �Դϴ� ���� �׻� WSA_WAIT_EVENT_0 �����༭ �ϳ� �������.
    index -= WSA_WAIT_EVENT_0;

    // ������ ���� ����
    // WSAEnumNetworkEvents �Լ��� ���Ͽ� �߻��� ��Ʈ��ũ �̺�Ʈ�� Ȯ���ϴ� �� ���
    // �ַ� WSAEvent�� �Բ� ���Ǿ� ���Ͽ��� �߻��� �̺�Ʈ�� �ִ��� Ȯ���ϰ� 
    // �̺�Ʈ�� ���¸� ��� �� Ȱ��
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

        // ������ ���� ��
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return false;
        }
        cout << "Recv: " << buffer << endl;
    }

    // ������� ����: �̺�Ʈ ��ü ���� Ȯ��(WSAWaitForMultipleEvents)
    // -> ���Ͽ��� �߻��� ��Ʈ��ũ �̺�Ʈ�� ���� ���� ���(WSAEnumNetworkEvents)


    //// receive ���� �о�ð� �ִٸ�
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

    //    // ������ ���� ��
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

    // send ������.
    int sendLen = ::send(m_ClientWinSock->GetSocket(), m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
    if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
        cout << "send Error " << ::WSAGetLastError() << endl;
    }
    else
    {
        cout << "send " << sendLen << " " << endl;
    }
}