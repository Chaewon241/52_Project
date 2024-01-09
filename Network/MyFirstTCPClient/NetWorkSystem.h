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
	// ���⿡ Ŭ���̾�Ʈ �ϳ� ���� �־����
	// winsockclient�� �ִ� doupdate�� ���⼭ ����.
	// PostMsg, PopMsg�� ���ؼ� client�� sendBuffer�� recvBuffer�� ��������.
	// (PopMsg�� client���� ȣ���ؼ� recvBuffer�� �������ָ� �ɵ�.)
	// �̱�������.
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

