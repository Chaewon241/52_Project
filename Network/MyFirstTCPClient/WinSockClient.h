#pragma once

#include <process.h>
#include <winsock2.h>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

class MyWinSocket;

// 클라이언트에서 입력 받고 어떤 것을 입력 받았는지 확인한 다음에 통신이 필요한 입력이면
// Send 시켜줌.
//

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

	// 클라이언트에 이벤트가 발생됐는지.
	int isEventOccurs();

	std::vector<WSAEVENT>& GetWSAEvent() { return wsaEvents; }

	void OnConnect(int nErrorCode);

	void OnReceive(int nErrorCode);

	SOCKET GetSocket() { return m_ClientWinSock->GetSocket(); }

private:
	bool m_isConnected = false;
	bool m_bAuth = false;

	char* m_sendBuffer;
	char* m_recvBuffer;

	MyWinSocket* m_ClientWinSock = nullptr;

	std::vector<WSAEVENT> wsaEvents;
};