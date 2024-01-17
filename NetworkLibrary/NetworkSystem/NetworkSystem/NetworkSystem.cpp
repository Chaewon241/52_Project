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

void NetworkSystem::OnReceive()
{
    char* recvBuffer = new char[RCV_BUF_SIZE];
    int recvBytes = ::recv(m_socket->GetSocket(), recvBuffer, RCV_BUF_SIZE, 0);

    if (recvBytes == SOCKET_ERROR)
    {
        //cout << "Read Error" << endl;
        return;
    }
    // 읽을거 없을 때
    if (recvBytes == 0)
    {
        //cout << "Disconnected" << endl;
        return;
    }

    //m_RecvQueue->enQueue(recvBuffer);
    //memcpy(m_recvBuffer + m_recvBytes, recvBuffer, recvBytes);

    //m_recvBytes += recvBytes;
}

void NetworkSystem::PostMsg(char* str, const int size)
{
}

PacketS2C_BroadcastMsg* NetworkSystem::PopMsg()
{
    return nullptr;
}