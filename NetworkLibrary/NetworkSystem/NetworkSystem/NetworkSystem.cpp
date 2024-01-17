#include "NetworkSystem.h"
#include "WinSock.h"

void NetworkSystem::Initialize(WinSock* socket)
{
	// initialize해줄 때 소켓을 멤버변수에 바인딩해주기.
	WSAData wsaData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	m_socket = socket;
}

void NetworkSystem::CleanUpSock()
{
    ::WSACleanup();
}

void NetworkSystem::DestroyInstance()
{
    delete m_NetWorkSystemInstance;
    m_NetWorkSystemInstance = nullptr;

    delete m_Client;
    m_Client = nullptr;

    delete m_sendBuffer;
    m_sendBuffer = nullptr;

    delete m_recvBuffer;
    m_recvBuffer = nullptr;

    delete m_RecvQueue;
    m_RecvQueue = nullptr;

    delete m_SendQueue;
    m_SendQueue = nullptr;
}

void NetworkSystem::PostMsg(char* str, const int size)
{
}

PacketS2C_BroadcastMsg* NetworkSystem::PopMsg()
{
    return nullptr;
}