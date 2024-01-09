#pragma once

#include "MyProtocol.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

class WinSockClient;

class NetWorkSystem
{
	// 여기에 클라이언트 하나 갖고 있어야함
	// winsockclient에 있던 doupdate를 여기서 해줌.
	// PostMsg, PopMsg를 통해서 client의 sendBuffer와 recvBuffer에 복사해줌.
	// (PopMsg는 client에서 호출해서 recvBuffer에 복사해주면 될듯.)
	// 싱글턴으로.
	//

public:
	void Initialize();
	void SetClient(WinSockClient* Client);

	void DoUpdate();
	void NetUpdate();

	static NetWorkSystem* GetNetWorkSystemInstance();

	void PostMsg(char* str, const int size);
	PacketS2C_BroadcastMsg* PopMsg();

	void DestroyInstance();

private:
	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;

	int m_sendBytes = 0;
	int m_recvBytes = 0;

	static NetWorkSystem* m_NetWorkSystemInstance;
	WinSockClient* m_Client;

	bool m_bConnected = false;
};

