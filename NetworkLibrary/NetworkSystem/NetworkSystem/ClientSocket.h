#pragma once
#include "WinSock.h"

typedef int SessionId;

class ClientSocket : public WinSock
{
public:
    void SetSessionId(SessionId sessionId) { m_sessionId = sessionId; };

    virtual void Attach(SOCKET socket) override;
    virtual void SetIp(std::string ip) override;
    virtual void SetPortNum(int port) override;

    virtual void OnAccept(int nErrorCode) override {};
    virtual void OnConnect(int nErrorCode) override {};
    virtual void OnSend(int nErrorCode) override {};
    virtual void OnReceive(int nErrorCode) override {};
    virtual void OnClose(int nErrorCode) override {};

private:
    std::string m_ip;
    int	m_port = 0;
    SessionId m_sessionId = 0;
};

