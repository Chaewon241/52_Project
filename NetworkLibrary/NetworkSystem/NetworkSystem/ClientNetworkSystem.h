#pragma once

#include "MyProtocol.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

class ClientNetworkSystem
{
public:
	void Initialize();
	void SetClient(WinSockClient* Client);

	void DoUpdate();
	void OnReceive();
	void NetUpdate();

	static ClientNetworkSystem* GetNetWorkSystemInstance();

	void PostMsg(char* str, const int size);
	PacketS2C_BroadcastMsg* PopMsg();

	void DestroyInstance();

private:
	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;

	// 원형큐가 있으면 얘네들이 필요없을 것 같음.
	int m_sendBytes = 0;
	int m_recvBytes = 0;

	static NetWorkSystem* m_NetWorkSystemInstance;
	WinSockClient* m_Client;

	MyCircularQueue* m_RecvQueue;
	MyCircularQueue* m_SendQueue;

	bool m_bConnected = false;
};

