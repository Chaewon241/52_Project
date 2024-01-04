#pragma once

#include "WinSock.h"

#include <process.h>
#include <winsock2.h>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

class MyWinSocket;

// Ŭ���̾�Ʈ���� �Է� �ް� � ���� �Է� �޾Ҵ��� Ȯ���� ������ ����� �ʿ��� �Է��̸�
// Send ������.
//

class WinSockClient
{
public:
	WinSockClient() {}
	virtual ~WinSockClient() 
	{
		delete m_ClientWinSock;
	}

	bool Initialize();

	bool Connect();

	void DisConnect();

	int Send(char* sendBuffer, int size);

	bool Update();

	//void NetUpdate();

	void CloseSocket();

	// Ŭ���̾�Ʈ�� �̺�Ʈ�� �߻��ƴ���.
	int EventState();

	std::vector<WSAEVENT>& GetWSAEvent() { return wsaEvents; }

	// NetworkSystem�� �Űܳ���
	/*void OnConnect(int nErrorCode);

	void OnReceive(int nErrorCode);*/

	SOCKET GetSocket() { return m_ClientWinSock->GetSocket(); }

private:
	bool m_isConnected = false;
	bool m_bAuth = false;

	MyWinSocket* m_ClientWinSock = nullptr;

	std::vector<WSAEVENT> wsaEvents;
};