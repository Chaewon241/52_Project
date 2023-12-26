#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")


//// �� Ŭ���̾�Ʈ�� ���� ���� ���� �Ǵ� ��ȣ�ۿ�
//struct Session
//{
//    SOCKET socket = INVALID_SOCKET; // ���� �ڵ�
//
//    char recvBuffer[BUFSIZE] = {};
//
//    int recvBytes = 0;
//    int sendBytes = 0; // ������ŭ ���� �ִ� �ΰ����� ���� ����
//};

// ���� ������ Winsock API ���� Ŭ������ ������.
// ���������� �ʿ��� �����Ϳ� �Լ��� �����س���.
// �갡 ���� CAsyncSocket
class WinSock // �츮�� �ϴ� �񵿱� ������ �������ϱ� ������ WSAEventSelect�� ����Ѵ�.
{
public:
    // ���뼺
        // ���� API
        // �񵿱� �����̹Ƿ� ������ ��, �ʿ��� ��Ʈ��ũ �̺�Ʈ�� ���� �ϵ��� �Ѵ�.
        // ���� ���� Create : long lEvent
    bool Create(int nSocketType);
    // �ּ� ���ε�
    bool Bind(UINT nSocketPort);
    // listen
    bool Listen(int nConnectionBacklog);
    // ���� Connect
    bool Connect(UINT nSocketPort, const char* nIP);
    bool DisConnect();
    // �ݱ� closesocket : ���� ���� ���� �ɼ��� �ִ�. ����Ʈ �ɼ����� �ݴ´�.

    // Accept
    // API�� �����ؼ� �����ΰ� ��� ���� Ŭ������ OnAccept���� �ش� �Լ��� ȣ��

    // Attach�� Detach�� �ʿ��� ����
    // ���� Ǯ�� ���� ��(���� �ڵ� ������) �̸� ���� 
    // Accept�� ����� ���� �ڵ��� �޾Ƽ� ���� ��ü Ǯ���� �ϳ� ������ Attach(���ε�)�ϰ�
    // ������ ������ �ش� ���� �ڵ��� ���̻� ������� ������ ��ü�� �ٽ� ����ؾ��ϹǷ�
    // Detach�ϰ� Ǯ�� �����ش�.
    // 

    bool CreateEventHandle();

    WSAEVENT GetEventHandle()
    {
        return m_WSAEvent;
    }

    SOCKET GetSocket()
    {
        return m_hSocket;
    }

    virtual void OnAccept(int nErrorCode) {};      // ���� �������� ������µ�
    virtual void OnClose(int nErrorCode) {};
    virtual void OnConnect(int nErrorCode) {};
    virtual void OnReceive(int nErrorCode) {};
    virtual void OnSend(int nErrorCode) {};

protected:
    SOCKET m_hSocket;

    WSAEVENT m_WSAEvent;
};


class MyWinSocket : public WinSock
{
public:
    virtual void OnReceive(int nErrorCode) {}

    // ���� ��Ŷ�� �м��ؼ� ó���� ������ �޽����� �и��ϰ�
    // ť�� �ִ´�. ť�� ó���� ������ �޽����� �ϼ��Ǿ��ٸ�.
    // 
    // OnMsgProc(&msg)
    //

    virtual void OnAccept(int nErrorCode) {};
    virtual void OnClose(int nErrorCode) {};
    virtual void OnConnect(int nErrorCode);
    virtual void OnSend(int nErrorCode);


    void OnMsgProc(void* pMsg)
    {
        // msg�� ����� ��Ŷ ���̵� Ȯ���ϰ� ������ �°� ó���ϴ� �Լ��� ȣ���Ѵ�.
    }
};