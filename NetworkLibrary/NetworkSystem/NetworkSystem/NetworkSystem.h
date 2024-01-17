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
	// ����ť
	
	// sendBuffer, recvBuffer
	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;
	// ���� �ϳ� ����ֱ�
	WinSock* m_socket;
};

