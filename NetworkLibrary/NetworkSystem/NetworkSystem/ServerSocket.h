#pragma once
#include "WinSock.h"
#include "Server.h"

class ServerSocket : public WinSock
{
public:
    ServerSocket(Server* sever);
    ~ServerSocket();

    virtual bool OnAccept() override;
    virtual void OnConnect(int nErrorCode) override {};
    virtual void OnSend(int nErrorCode) override {};
    virtual void OnReceive(int nErrorCode) override {};
    virtual void OnClose(int nErrorCode) override {};

private:
    Server* m_server;
};


