#pragma once

#include "MyProtocol.h"

class WinSock;

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

class NetworkSystem
{
public:
	void Initialize(WinSock* socket);
	void DestroyInstance();

	void OnReceive();

	void PostMsg(char* str, const int size);
	PacketS2C_BroadcastMsg* PopMsg();

	static NetworkSystem* GetNetWorkSystemInstance();
	
private:
	// 원형큐
	
	// sendBuffer, recvBuffer
	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;
	// 소켓 하나 들고있기
	WinSock* m_socket;
};

