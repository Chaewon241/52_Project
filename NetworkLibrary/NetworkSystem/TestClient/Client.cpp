#include "Client.h"
#include "ClientSocket.h"
#include "WinSock.h"

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

void Client::Start()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

	m_clientSocket = new ClientSocket;
    if (!m_clientSocket->Create() || !m_clientSocket->Connect("127.0.0.1", 7777))
        return;

    m_sendBuffer = new char[SND_BUF_SIZE];
    m_recvBuffer = new char[RCV_BUF_SIZE];

    memcpy(m_sendBuffer, "aaa" + '\0', 4);

    m_RecvQueue = new Curtaincall::CircularQueue(100);
    m_SendQueue = new Curtaincall::CircularQueue(100);

    m_SendQueue->enQueue(m_sendBuffer);
}

void Client::Update()
{
    WSAEVENT clientEvent = m_clientSocket->GetEvent();
    int index = ::WSAWaitForMultipleEvents(1, &clientEvent, FALSE, 1, FALSE);

    if (index == WSA_WAIT_FAILED)
        return;

    index -= WSA_WAIT_EVENT_0;

    if (index == WSA_WAIT_TIMEOUT)
        return;

    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(m_clientSocket->GetHandle(), clientEvent, &networkEvents) == SOCKET_ERROR)
        return;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Connect", m_clientSocket);
            return;
        }
        return;
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Read", m_clientSocket);
            return;
        }
        OnReceive();
    }

    if (networkEvents.lNetworkEvents & FD_WRITE)
    {
        if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", m_clientSocket);
            return;
        }

        OnSend();
    }

    if (networkEvents.lNetworkEvents & FD_CLOSE)
    {
        if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
        {
            return;
        }
    }
}

void Client::NetUpdate()
{
    if (m_SendQueue->isEmpty())
        return;
    int nSent = m_clientSocket->Send(m_SendQueue->Peek(), strlen(m_sendBuffer) + 1);
    if (nSent > 0)
    {
        m_SendQueue->deQueue();

    }
    else if (nSent == 0)
    {
        // send 할 수 없는 상태인지 아닌지 변수 만들기?
        // 근데 이렇게 되면 서버가 언제 준비되는지 모름.
    }
}

void Client::OnReceive()
{
    char* recvBuffer = new char[RCV_BUF_SIZE];
    int recvBytes = m_clientSocket->Recv(recvBuffer, RCV_BUF_SIZE);

    if (recvBytes == SOCKET_ERROR)
    {
        return;
    }
    // 읽을거 없을 때
    if (recvBytes == 0)
    {
        return;
    }
    m_RecvQueue->enQueue(recvBuffer);
    printf("readBuffer: %s \n", recvBuffer);
}

void Client::OnSend()
{
    
}

void Client::OnClose()
{
}

void Client::Stop()
{
    delete m_clientSocket;
    m_clientSocket = nullptr;

    delete m_sendBuffer;
    m_sendBuffer = nullptr;

    delete m_recvBuffer;
    m_recvBuffer = nullptr;

    delete m_RecvQueue;
    m_RecvQueue = nullptr;

    delete m_SendQueue;
    m_SendQueue = nullptr;

    ::WSACleanup();
}

void Client::OnNetError(int errorCode, const char* errorMsg, WinSock* pSocket)
{
    if (errorMsg)
    {
        printf("\n onNetError %s \t", errorMsg);
    }

    if (pSocket)
    {
        printf("\n onNetError  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());
    }

    printf("NetErrorCode  %d  \n", errorCode);
}

void Client::ClientLoop()
{
	Start();
	while(true)
	{
        Update();
        NetUpdate();
	}
    Stop();
}
