#pragma once

#include "../MyFirstTcpServer/Types.h"

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
	void DoUpdate();

	static NetWorkSystem* GetNetWorkSystemInstance();

	void PostMsg(netfish::NetMsg* pMsg, const int size);
	netfish::NetMsg* PopMsg();

	void DestroyInstance();

private:
	char* m_sendBuffer;
	char* m_recvBuffer;

	static NetWorkSystem* m_NetWorkSystemInstance;
	WinSockClient* m_Client;

	bool m_bConnected = false;
};

