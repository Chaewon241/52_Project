#pragma once

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
		delete m_sendBuffer;
		delete m_recvBuffer;
	}

	bool Initialize();

	bool Connect();

	void DisConnect();

	int Send();

	bool Update();

	void NetUpdate();

	void SendAllChatting(char* inputStr);

	void RecvReadBuffer(char* readBuffer);
	void WriteSendBuffer(char* sendBuffer, int size);

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

	char* m_sendBuffer;
	char* m_recvBuffer;

	int m_writeBytes = 0;
	int m_readBytes = 0;

	MyWinSocket* m_ClientWinSock = nullptr;

	std::vector<WSAEVENT> wsaEvents;
};