#pragma once
#include "WinSock.h"

class ServerSocket : public WinSock
{
public:
	ServerSocket() = default;
	~ServerSocket() = default;

	virtual bool OnAccept(WinSock* pSocket) override
	{
		return (Accept(*pSocket));
	}
	virtual bool OnConnect() override { return false; }
	virtual bool OnClose() override { return false; }
	virtual bool OnSend() override { return false; }
	virtual bool OnReceive() override { return false; }
};


