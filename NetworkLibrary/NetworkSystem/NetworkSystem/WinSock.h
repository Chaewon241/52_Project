#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

class WinSock
{
public:
    // ����
    bool Create(int nSocketType = SOCK_STREAM,
        long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
    SOCKET GetSocket() { return m_hSocket; }

    // ����
    bool Bind(UINT nSocketPort);
    bool Listen(int nConnectionBacklog = SOMAXCONN);

    // Ŭ��
    bool Connect(UINT nSocketPort, const char* nIP);
    bool DisConnect();
    virtual void Attach(SOCKET socket) {};
    virtual void SetIp(std::string ip) {};
    virtual void SetPortNum(int port) {};

    // �̺�Ʈ ����
    void AddEvent(WSAEVENT hEvent) { m_WSAEvents.push_back(hEvent); }
    std::vector<WSAEVENT>& GetWSAEvents() { return m_WSAEvents; }
    int GetEventIndex();


    virtual bool OnAccept() {};
    virtual void OnClose(int nErrorCode) {};
    virtual void OnConnect(int nErrorCode) {};
    virtual void OnReceive(int nErrorCode) {};
    virtual void OnSend(int nErrorCode) {};

protected:
    SOCKET m_hSocket;
    std::vector<WSAEVENT> m_WSAEvents;
    int m_error;
};



