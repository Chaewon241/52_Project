#pragma once
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
	void DoUpdate(SOCKET socket, int index);
	static NetWorkSystem* GetNetWorkSystemInstance();

	void DestroyInstance();

private:
	static NetWorkSystem* m_NetWorkSystemInstance;
	WinSockClient* m_Client;
};

