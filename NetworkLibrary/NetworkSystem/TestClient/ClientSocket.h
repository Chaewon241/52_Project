#pragma once
#include "WinSock.h"

typedef int SessionId;

class ClientSocket : public WinSock
{
public:
	ClientSocket() = default;
	~ClientSocket() = default;

	virtual bool OnAccept(WinSock* pSocket) override { return false; }
	virtual bool OnConnect() override { return false; }
	virtual bool OnClose() override { return false; }
	virtual bool OnSend() override { return false; }
	virtual bool OnReceive() override { return false; }

};

