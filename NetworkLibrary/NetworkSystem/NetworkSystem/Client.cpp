#include "Client.h"
#include "ClientSocket.h"

void Client::Start()
{
	m_clientSocket = new ClientSocket;
    if (!m_clientSocket->Create() || !m_clientSocket->Connect("127.0.0.1", 7777))
        return;
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
            return;
        }
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            return;
        }
        OnReceive();
    }

    if (networkEvents.lNetworkEvents & FD_WRITE)
    {
        if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
        {
            //onNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", pSocket);
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

void Client::OnReceive()
{
    m_clientSocket->Recv();
}

void Client::OnSend()
{
}

void Client::Stop()
{
}

void Client::ClientLoop()
{
	Start();
	m_networkSystem.Initialize(m_clientSocket);

	while(true)
	{
        Update();
	}
    m_networkSystem.CleanUpSock();
}
