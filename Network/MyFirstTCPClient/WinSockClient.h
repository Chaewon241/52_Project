#pragma once

#include <process.h>
#include <winsock2.h>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

class MyWinSocket;

class WinSockClient
{
public:
	WinSockClient() {}
	virtual ~WinSockClient() 
	{
		delete m_ClientWinSock;
		delete m_sendBuffer;
		delete m_recvBuffer;
	}

	bool Initialize();

	bool Connect();

	void DisConnect();

	void Send();

	bool DoUpdate();

	void PressKey();

	void CloseSocket();

private:

	void OnConnect(int nErrorCode);

private:
	bool m_isConnected = false;
	bool m_bAuth = false;

	char* m_sendBuffer;
	char* m_recvBuffer;

	MyWinSocket* m_ClientWinSock = nullptr;

	std::vector<WSAEVENT> wsaEvents;
};