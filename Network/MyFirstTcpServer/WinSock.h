#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

constexpr int BUFSIZE = 1024;

namespace net_global
{
    //bool WinsockInit()
    //{
    //    /*if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    //        return false;*/

    //    return true;
    //}

    //void WinsockCleanUp()
    //{

    //}
}

// �� Ŭ���̾�Ʈ�� ���� ���� ���� �Ǵ� ��ȣ�ۿ�
struct Session
{
    SOCKET socket = INVALID_SOCKET; // ���� �ڵ�

    char recvBuffer[BUFSIZE] = {};

    int recvBytes = 0;
    int sendBytes = 0; // ������ŭ ���� �ִ� �ΰ����� ���� ����
};

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
    bool Connect(UINT nSocketPort);
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
    int WaitForMultipleEvents();
    bool EnumNetworkEvents();

    UINT GetEventState() { return networkEvents.lNetworkEvents; }
    int* GetErrorCode() { return networkEvents.iErrorCode; }




    // ���� ���� : WinSocket���� ��ü�� ������ �̸� ������ �� �ִ�. ���� ��ü Ǯ
    // ���� ������. (socket(..))

// ������
    virtual void OnAccept(int nErrorCode) = 0;      // ���� �������� ������µ�
    //virtual void OnClose(int nErrorCode) = 0;
    virtual void OnConnect(int nErrorCode) = 0;
    virtual void OnReceive(int nErrorCode) = 0;
    virtual bool OnSend(int nErrorCode) = 0;

protected:
    SOCKET m_hSocket;

    vector<WSAEVENT> wsaEvents;
    vector<Session> sessions;

    // �̺�Ʈ�� �ε���
    int index;

    // ������ �̺�Ʈ ����, �̺�Ʈ�� ���� ���� �ڵ� �迭�� ����.
    WSANETWORKEVENTS networkEvents;
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

    virtual void OnAccept(int nErrorCode);
    //virtual void OnClose(int nErrorCode);
    virtual void OnConnect(int nErrorCode) {}
    virtual bool OnSend(int nErrorCode);


    void OnMsgProc(void* pMsg)
    {
        // msg�� ����� ��Ŷ ���̵� Ȯ���ϰ� ������ �°� ó���ϴ� �Լ��� ȣ���Ѵ�.
    }
};